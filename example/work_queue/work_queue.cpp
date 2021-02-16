// Copyright 2011-2012 Renato Tegon Forti
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// -----------------------------------------------------------------------------
// This example shows how implement a work quee (thread pool) to work with
// Boost.Application using Boost.Asio.
//
// The result will be printed on CTRL-C (Stop) signal
// -----------------------------------------------------------------------------

#include <boost/application.hpp>
#include <boost/timer/timer.hpp>

#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <functional>
#include <mutex>

#include "work_queue.hpp"

using namespace boost;

using matrix_type = std::vector<std::vector<double>>;

// worker class that calculate gaussian blur
// http://en.wikipedia.org/wiki/Gaussian_blur
template< int kernelRadius = 3> 
struct gaussian_blur
{
   using callback = std::function<void(const matrix_type&)>;

   explicit gaussian_blur(callback cb)
      : callback_(std::move(cb))
   {
   }

   void operator()()
   {
      boost::timer::cpu_timer timer;

      kernel2d_ = produce_gaussian_kernel(kernelRadius);

      boost::timer::cpu_times const elapsed_times(timer.elapsed());

      std::cout 
         << "gaussian_blur takes:"
         <<  format(elapsed_times, 9)
         << ", for size: "
         << kernelRadius
         << std::endl;

      callback_(kernel2d_);
   }

protected:

   double gaussian(double x, double mu, double sigma)
   {
      return exp(-(((x-mu)/(sigma))*((x-mu)/(sigma)))/2.0);
   }

   matrix_type produce_gaussian_kernel(int internalKernelRadius)
   {
      // get kernel matrix
      matrix_type kernel2d(2*internalKernelRadius+1, std::vector<double>(2*internalKernelRadius+1));

      // determine sigma
      double sigma = internalKernelRadius/2.;

      // fill values
      double sum = 0;
      for (std::size_t row = 0; row < kernel2d.size(); row++)
      {
         for (std::size_t col = 0; col < kernel2d[row].size(); col++)
         {
            kernel2d[row][col] = gaussian(row, internalKernelRadius, sigma) * gaussian(col, internalKernelRadius, sigma);
            sum += kernel2d[row][col];
         }
      }

      // normalize kernel, or the image becomes dark 
      for (auto & row : kernel2d)
         for (double & col : row)
            col /= sum;

      return kernel2d;
   }

private:

   callback callback_;
   matrix_type kernel2d_;
};

// application class
class myapp : work_queue<0>
{
public:

   explicit myapp(application::context& context)
      : task_count_(0), context_(context)
   {
   }

   void add_result(const matrix_type& kernel2d)
   {
      std::lock_guard<std::mutex> lock(mutex_);

      task_count_++;

      result_.push_back(kernel2d);

      if(task_count_== 3)
      {
         std::cout << "all tasks are completed, waiting ctrl-c to display the results..." << std::endl;
      }
   }

   int operator()()
   {
      // your application logic here!
      task_count_ = 0;

      //our tasks
      add_task(gaussian_blur<3>([this](const matrix_type& kernel2d) { add_result(kernel2d); }));
      add_task(gaussian_blur<6>([this](const matrix_type& kernel2d) { add_result(kernel2d); }));
      add_task(gaussian_blur<9>([this](const matrix_type& kernel2d) { add_result(kernel2d); }));

      context_.find<application::wait_for_termination_request>()->wait();

      return 0;
   }

   bool stop()
   {
      std::cout << "Result..." << std::endl;

      for(std::size_t i = 0; i < result_.size(); ++i)
      {
         std::cout << i << " : -----------------------" << std::endl;

         auto& kernel2d = result_[i];

         for (auto & row : kernel2d) {
            for (double col : row) {
               std::cout << std::setprecision(5) << std::fixed << col << " ";
            }
            std::cout << std::endl;
         }
      }

      return true;
   }

private:

   std::mutex mutex_;
   std::vector<std::vector<std::vector<double>>> result_;

   int task_count_;

   application::context& context_;
   
}; // myapp 

int main(int /*argc*/, char */*argv*/[])
{
   application::context app_context;
   myapp app(app_context);

   application::handler<>::callback cb = [&app] { return app.stop(); };

   app_context.insert<application::termination_handler>(
      std::make_shared<application::termination_handler_default_behaviour>(cb));

   return application::launch<application::common>(app, app_context);
}
