﻿#ifndef LIGHTYEAR_PB_DISPATCHER_HPP
#define LIGHTYEAR_PB_DISPATCHER_HPP
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "log.hpp"
#include <google/protobuf/message.h>
#include <map>


class Callback : boost::noncopyable
{
public:
    virtual ~Callback() {};
    virtual void onMessage(google::protobuf::Message* message) const = 0;
};


template <typename T>
class CallbackT : public Callback
{
public:
    typedef boost::function<void (T* message)> ProtobufMessageCallback;

    CallbackT(const ProtobufMessageCallback& callback)
        : callback_(callback)
    {
    }

    virtual void onMessage(google::protobuf::Message* message) const
    {
        T* t = dynamic_cast<T*>(message);
        assert(t != NULL);
        callback_(t);
    }

private:
    ProtobufMessageCallback callback_;
};

inline void discardProtobufMessage(google::protobuf::Message* message)
{
    LOG(logERROR) << "Discarding " << message->GetTypeName();
}

class ProtobufDispatcher
{
public:

    ProtobufDispatcher()
        : defaultCallback_(discardProtobufMessage)
    {
    }

    void onMessage(google::protobuf::Message* message) const
    {
        CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
        if (it != callbacks_.end())
        {
            it->second->onMessage(message);
        }
        else
        {
            defaultCallback_(message);
        }
    }

    template<typename T>
    void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageCallback& callback)
    {
        boost::shared_ptr<CallbackT<T> > pd(new CallbackT<T>(callback));
        callbacks_[T::descriptor()] = pd;
    }

    typedef std::map<const google::protobuf::Descriptor*, boost::shared_ptr<Callback> > CallbackMap;
    CallbackMap callbacks_;
    boost::function<void (google::protobuf::Message* message)> defaultCallback_;
};


#endif