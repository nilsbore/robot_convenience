# robot_convenience
C++/Python convenience libraries for working with Eigen, PCL, ROS, GraphViz, mongodb_store etc.

## eigen_cereal

Package for easily serealizing Eigen matrices and other data types using the C++ Cereal library.

## ros_cereal

Package for easily serializing ROS message types using the C++ Cereal library.

## simple_graph

A header based graph library that aims to be very barebones, and enable simple serialization into the GraphViz `.dot` format.

## mongodb_store_associated

This is a wrapper around the mongodb_store classes. Mongodb_store enables easy insertion and querying of ROS messages into MongoDB. The problem is that MongoDB has a hard 16 MB limit on message size. This library solves this by enabling messages to have associated messages, spreading the message sizes between different MongoDB documents.
