/* Auto-generated by genmsg_cpp for file /home/mechatronica/rosbee/packages/rosbee_control/msg/encoders.msg */
#ifndef ROSBEE_CONTROL_MESSAGE_ENCODERS_H
#define ROSBEE_CONTROL_MESSAGE_ENCODERS_H
#include <string>
#include <vector>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/message.h"
#include "ros/time.h"


namespace rosbee_control
{
template <class ContainerAllocator>
struct encoders_ : public ros::Message
{
  typedef encoders_<ContainerAllocator> Type;

  encoders_()
  : leftEncoder(0)
  , rightEncoder(0)
  {
  }

  encoders_(const ContainerAllocator& _alloc)
  : leftEncoder(0)
  , rightEncoder(0)
  {
  }

  typedef uint16_t _leftEncoder_type;
  uint16_t leftEncoder;

  typedef uint16_t _rightEncoder_type;
  uint16_t rightEncoder;


private:
  static const char* __s_getDataType_() { return "rosbee_control/encoders"; }
public:
  ROS_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROS_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "1bdb5ccd88c86c9ee03f3c16a809db53"; }
public:
  ROS_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROS_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "uint16 leftEncoder\n\
uint16 rightEncoder\n\
\n\
"; }
public:
  ROS_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROS_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    ros::serialization::serialize(stream, leftEncoder);
    ros::serialization::serialize(stream, rightEncoder);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    ros::serialization::deserialize(stream, leftEncoder);
    ros::serialization::deserialize(stream, rightEncoder);
    return stream.getData();
  }

  ROS_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    size += ros::serialization::serializationLength(leftEncoder);
    size += ros::serialization::serializationLength(rightEncoder);
    return size;
  }

  typedef boost::shared_ptr< ::rosbee_control::encoders_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::rosbee_control::encoders_<ContainerAllocator>  const> ConstPtr;
}; // struct encoders
typedef  ::rosbee_control::encoders_<std::allocator<void> > encoders;

typedef boost::shared_ptr< ::rosbee_control::encoders> encodersPtr;
typedef boost::shared_ptr< ::rosbee_control::encoders const> encodersConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::rosbee_control::encoders_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::rosbee_control::encoders_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace rosbee_control

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator>
struct MD5Sum< ::rosbee_control::encoders_<ContainerAllocator> > {
  static const char* value() 
  {
    return "1bdb5ccd88c86c9ee03f3c16a809db53";
  }

  static const char* value(const  ::rosbee_control::encoders_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x1bdb5ccd88c86c9eULL;
  static const uint64_t static_value2 = 0xe03f3c16a809db53ULL;
};

template<class ContainerAllocator>
struct DataType< ::rosbee_control::encoders_<ContainerAllocator> > {
  static const char* value() 
  {
    return "rosbee_control/encoders";
  }

  static const char* value(const  ::rosbee_control::encoders_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::rosbee_control::encoders_<ContainerAllocator> > {
  static const char* value() 
  {
    return "uint16 leftEncoder\n\
uint16 rightEncoder\n\
\n\
";
  }

  static const char* value(const  ::rosbee_control::encoders_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::rosbee_control::encoders_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::rosbee_control::encoders_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.leftEncoder);
    stream.next(m.rightEncoder);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct encoders_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::rosbee_control::encoders_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::rosbee_control::encoders_<ContainerAllocator> & v) 
  {
    s << indent << "leftEncoder: ";
    Printer<uint16_t>::stream(s, indent + "  ", v.leftEncoder);
    s << indent << "rightEncoder: ";
    Printer<uint16_t>::stream(s, indent + "  ", v.rightEncoder);
  }
};


} // namespace message_operations
} // namespace ros

#endif // ROSBEE_CONTROL_MESSAGE_ENCODERS_H

