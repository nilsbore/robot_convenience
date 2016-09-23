#ifndef ROS_CEREAL_H
#define ROS_CEREAL_H

#include <cereal/cereal.hpp>
#include <fstream>
#include <ros/serialization.h>

namespace cereal
{

/*
template<typename T>
struct has_ros_serialize
{
    template<typename U, void (U::*)(uint8_t*) const> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, &U::serialize>*);
    template<typename U> static int Test(...);
    static const bool value = sizeof(Test<T>(0)) == sizeof(char);
};

template<typename T>
struct has_ros_deserialize
{
    template<typename U, void (U::*)(uint8_t*) const> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, &U::deserialize>*);
    template<typename U> static int Test(...);
    static const bool value = sizeof(Test<T>(0)) == sizeof(char);
};
*/

/*
template<typename T>
struct is_ros_serializable
{
    template<typename U, uint32_t (U::*)(const T&)> struct SFINAE {};
    template<typename U> static char Test(SFINAE<U, &U::serializedLength>*);
    template<typename U> static int Test(...);
    static const bool value = sizeof(Test<ros::serialization::Serializer<T> >(0)) == sizeof(char);
};
*/

template <class Archive, class Msg> inline
typename std::enable_if<std::is_default_constructible<ros::serialization::Serializer<std::vector<Msg> > >::value && traits::is_output_serializable<BinaryData<u_int8_t>, Archive>::value, void>::type
save(Archive& ar, std::vector<Msg> const & m)
{
    namespace ser = ros::serialization;

    size_t serial_size = ros::serialization::serializationLength(m);
    boost::shared_array<u_int8_t> buffer(new u_int8_t[serial_size]);

    ser::OStream stream(buffer.get(), serial_size);
    ser::serialize(stream, m);

    ar(serial_size);
    ar(binary_data(buffer.get(), serial_size));
}

template <class Archive, class Msg> inline
typename std::enable_if<std::is_default_constructible<ros::serialization::Serializer<Msg> >::value && traits::is_output_serializable<BinaryData<u_int8_t>, Archive>::value, void>::type
save(Archive& ar, Msg const & m)
{
    namespace ser = ros::serialization;

    size_t serial_size = ros::serialization::serializationLength(m);
    boost::shared_array<u_int8_t> buffer(new u_int8_t[serial_size]);

    ser::OStream stream(buffer.get(), serial_size);
    ser::serialize(stream, m);

    ar(serial_size);
    ar(binary_data(buffer.get(), serial_size));
}

template <class Archive, class Msg> inline
typename std::enable_if<std::is_default_constructible<ros::serialization::Serializer<std::vector<Msg> > >::value && traits::is_input_serializable<BinaryData<u_int8_t>, Archive>::value, void>::type
load(Archive& ar, std::vector<Msg>& m)
{
    namespace ser = ros::serialization;

    //size_t serial_size = ros::serialization::serializationLength(m);
    size_t serial_size;
    ar(serial_size);

    boost::shared_array<u_int8_t> buffer(new u_int8_t[serial_size]);
    ar(binary_data(buffer.get(), serial_size));

    ser::IStream stream(buffer.get(), serial_size);
    ser::deserialize(stream, m);
}

template <class Archive, class Msg> inline
typename std::enable_if<std::is_default_constructible<ros::serialization::Serializer<Msg> >::value && traits::is_input_serializable<BinaryData<u_int8_t>, Archive>::value, void>::type
load(Archive& ar, Msg& m)
{
    namespace ser = ros::serialization;

    //size_t serial_size = ros::serialization::serializationLength(m);
    size_t serial_size;
    ar(serial_size);

    boost::shared_array<u_int8_t> buffer(new u_int8_t[serial_size]);
    ar(binary_data(buffer.get(), serial_size));

    ser::IStream stream(buffer.get(), serial_size);
    ser::deserialize(stream, m);
}

}

#endif // ROS_CEREAL_H
