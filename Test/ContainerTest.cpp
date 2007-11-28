#include <stddef.h>       // some older implementations lack <cstddef> 
#include <time.h> 
#include <math.h> 
#include <stdlib.h> 


#include <vector> 
#include <algorithm> 
#include <list> 
#include <deque> 
#include <set> 


#include <iostream> 
#include <iomanip> 


typedef double element_t; 


using namespace std; 


vector<double> result_times; // results are puched into this vector 


const char header[] = 
                "\tarray" 
                "\tvector with pointers" 
                "\tvector with iterators" 
                "\tdeque" 
                "\tlist" 
                "\tset" 
                "\tmultiset"; 


void do_head() 
{ 
        cout << "size" << header << '\n'; 



} 


int do_tail() 
{ 
        // in case you want to stop for confirmation in a windows console application 
        //char c; 
        //cin >> c; 
        return 0; 


} 


void do_row(int size) 
{ 
        cout << size; 
        cout << fixed << setprecision(2); 
        for (size_t i = 0; i < result_times.size(); ++i) 
                cout << '\t' << result_times[i]; 
        cout << '\n'; 


} 


clock_t start_time; 

inline void start_timer() { start_time = clock(); } 


inline double timer() 
{ 
  clock_t end_time = clock(); 
  return (end_time - start_time)/double(CLOCKS_PER_SEC); 



} 


typedef void(*Test)(element_t*, element_t*, int); 
void run(Test f, element_t* first, element_t* last, int number_of_times) 
{ 
        start_timer(); 
        while (number_of_times-- > 0) f(first,last,number_of_times); 
        result_times.push_back(timer()); 


} 


void array_test(element_t* first, element_t* last, int ) 
{ 
       element_t* array = new element_t[last - first]; 
       copy(first, last, array); 
       sort(array, array + (last - first)); 
       unique(array, array + (last - first)); 
       delete [] array;   


} 


void vector_pointer_test(element_t* first, element_t* last, int ) 
{ 
       vector<element_t> container(first, last); 
           // &*container.begin() gets us a pointer to the first element 
       sort(&*container.begin(), &*container.end()); 
       unique(&*container.begin(), &*container.end()); 


} 


void vector_iterator_test(element_t* first, element_t* last, int ) 
{ 
       vector<element_t> container(first, last); 
       sort(container.begin(), container.end()); 
       unique(container.begin(), container.end()); 


} 


void deque_test(element_t* first, element_t* last, int ) 
{   
//       deque<element_t> container(first, last); CANNOT BE USED BECAUSE OF MVC++ 6 
        deque<element_t> container(size_t(last - first), 0.0); 
        copy(first, last, container.begin()); 
        sort(container.begin(), container.end()); 
        unique(container.begin(), container.end()); 


} 


void list_test(element_t* first, element_t* last, int number_of_times) 
{ 
       list<element_t> container(first, last); 
       container.sort(); 
           container.unique(); 


} 


void set_test(element_t* first, element_t* last, int number_of_times) 
{ 
       set<element_t> container(first, last); 


} 


void multiset_test(element_t* first, element_t* last, int number_of_times) 
{ 
       multiset<element_t> container(first, last); 
       typedef multiset<element_t>::iterator iterator; 
           { 
                        iterator first = container.begin(); 
                        iterator last = container.end(); 

                        while (first != last) { 
                                iterator next = first; 
                                if (++next == last) break; 
                                if (*first == *next) 
                                        container.erase(next); 
                                else 
                                        ++first; 
                        } 
           } 



} 


void initialize(element_t* first, element_t* last) 
{ 
  element_t value = 0.0; 
  while (first != last) { 
         *first++ = value; 
         value += 1.; 
  } 


} 


double logtwo(double x) 
{ 
  return log(x)/log((double) 2.0); 


} 


const int largest_size = 1000000; 

int number_of_tests(int size) { 
        double n = size; 
        double largest_n = largest_size; 
        return int(floor((largest_n * logtwo(largest_n)) / (n * logtwo(n)))); 



} 


void run_tests(int size) 
{ 
        const int n = number_of_tests(size); 
        const size_t length = 2*size; 
        result_times.clear(); 

// make a random test set of the chosen size: 
  vector<element_t> buf(length); 
  element_t* buffer = &buf[0]; 
  element_t* buffer_end = &buf[length]; 
  initialize(buffer, buffer + size);            // elements 
  initialize(buffer + size, buffer_end);        // duplicate elements 
  random_shuffle(buffer, buffer_end); 


// test the containers: 
  run(array_test, buffer, buffer_end, n); 
  run(vector_pointer_test, buffer, buffer_end, n); 
  run(vector_iterator_test, buffer, buffer_end, n); 
  run(deque_test, buffer, buffer_end, n); 
  run(list_test, buffer, buffer_end, n); 
  run(set_test, buffer, buffer_end, n); 
  run(multiset_test, buffer, buffer_end, n); 
  do_row(size); 



} 


int SpeedTest() 
{ 
  do_head(); 
  const int sizes [] = {10, 100, 1000, 10000, 100000, 1000000}; 
  const int n = sizeof(sizes)/sizeof(int); 
  for (int i = 0; i < n; ++i) run_tests(sizes[i]); 
  return do_tail(); 
}  