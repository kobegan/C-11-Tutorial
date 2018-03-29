 cmake_minimum_required(VERSION 3.9)
 
 PROJECT(C++_11_tutorial)
 
# INCLUDE_DIRECTORIES("D:/wms-20171120/build/build/local/x86_64/include")
# INCLUDE_DIRECTORIES("D:/wms-20171120/build/build/local/x86_64/include/gstreamer-1.0")
# INCLUDE_DIRECTORIES("D:/wms-20171120/build/build/local/x86_64/include/glib-2.0")
# INCLUDE_DIRECTORIES("D:/wms-20171120/build/build/local/x86_64/lib/glib-2.0/include")

# LINK_DIRECTORIES("D:/wms-20171120/build/build/local/x86_64/lib")
 
 ADD_SUBDIRECTORY(multithreading)
 ADD_SUBDIRECTORY(stop_or_terminate_thread)
 ADD_SUBDIRECTORY(lambda)
 ADD_SUBDIRECTORY(smart_pointer)
 ADD_SUBDIRECTORY(std_array)
 ADD_SUBDIRECTORY(variadic_templates)
