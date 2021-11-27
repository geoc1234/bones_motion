#ifndef _ROS_bones_msgs_ardFeedback_h
#define _ROS_bones_msgs_ardFeedback_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace bones_msgs
{

  class ardFeedback : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef int32_t _msgtype_type;
      _msgtype_type msgtype;
      typedef int32_t _val_type;
      _val_type val;
      typedef float _p0_type;
      _p0_type p0;
      typedef float _p1_type;
      _p1_type p1;
      typedef float _p2_type;
      _p2_type p2;
      typedef float _p3_type;
      _p3_type p3;

    ardFeedback():
      header(),
      msgtype(0),
      val(0),
      p0(0),
      p1(0),
      p2(0),
      p3(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      union {
        int32_t real;
        uint32_t base;
      } u_msgtype;
      u_msgtype.real = this->msgtype;
      *(outbuffer + offset + 0) = (u_msgtype.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_msgtype.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_msgtype.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_msgtype.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->msgtype);
      union {
        int32_t real;
        uint32_t base;
      } u_val;
      u_val.real = this->val;
      *(outbuffer + offset + 0) = (u_val.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_val.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_val.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_val.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->val);
      union {
        float real;
        uint32_t base;
      } u_p0;
      u_p0.real = this->p0;
      *(outbuffer + offset + 0) = (u_p0.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_p0.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_p0.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_p0.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->p0);
      union {
        float real;
        uint32_t base;
      } u_p1;
      u_p1.real = this->p1;
      *(outbuffer + offset + 0) = (u_p1.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_p1.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_p1.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_p1.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->p1);
      union {
        float real;
        uint32_t base;
      } u_p2;
      u_p2.real = this->p2;
      *(outbuffer + offset + 0) = (u_p2.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_p2.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_p2.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_p2.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->p2);
      union {
        float real;
        uint32_t base;
      } u_p3;
      u_p3.real = this->p3;
      *(outbuffer + offset + 0) = (u_p3.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_p3.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_p3.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_p3.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->p3);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      union {
        int32_t real;
        uint32_t base;
      } u_msgtype;
      u_msgtype.base = 0;
      u_msgtype.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_msgtype.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_msgtype.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_msgtype.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->msgtype = u_msgtype.real;
      offset += sizeof(this->msgtype);
      union {
        int32_t real;
        uint32_t base;
      } u_val;
      u_val.base = 0;
      u_val.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_val.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_val.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_val.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->val = u_val.real;
      offset += sizeof(this->val);
      union {
        float real;
        uint32_t base;
      } u_p0;
      u_p0.base = 0;
      u_p0.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_p0.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_p0.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_p0.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->p0 = u_p0.real;
      offset += sizeof(this->p0);
      union {
        float real;
        uint32_t base;
      } u_p1;
      u_p1.base = 0;
      u_p1.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_p1.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_p1.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_p1.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->p1 = u_p1.real;
      offset += sizeof(this->p1);
      union {
        float real;
        uint32_t base;
      } u_p2;
      u_p2.base = 0;
      u_p2.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_p2.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_p2.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_p2.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->p2 = u_p2.real;
      offset += sizeof(this->p2);
      union {
        float real;
        uint32_t base;
      } u_p3;
      u_p3.base = 0;
      u_p3.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_p3.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_p3.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_p3.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->p3 = u_p3.real;
      offset += sizeof(this->p3);
     return offset;
    }

    virtual const char * getType() override { return "bones_msgs/ardFeedback"; };
    virtual const char * getMD5() override { return "f509fa12f565c8c7af76858b7f1a8abe"; };

  };

}
#endif
