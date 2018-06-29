#ifndef _HELLO_WORLD_PRODUCER_H_
#define _HELLO_WORLD_PRODUCER_H_

#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/System.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace activemq::core;
using namespace decaf::util::concurrent;
using namespace decaf::util;
using namespace decaf::lang;
using namespace cms;
using namespace std;


class HelloWorldProducer : public Runnable
{
 private:
    Connection *connection;
    Session *session;
    Destination *destination;
    MessageProducer *producer;
    int numMessages;
    bool useTopic;
    bool sessionTransacted;
    std::string brokerURI;

  private:
    HelloWorldProducer(const HelloWorldProducer&);
    HelloWorldProducer& operator=(const HelloWorldProducer&);

public:
    HelloWorldProducer(const std::string& brokerURI,
        int numMessages,bool useTopic = false,
        bool sessionTransacted = false):connection(NULL),
            session(NULL),destination(NULL),producer(NULL),
            numMessages(numMessages),useTopic(useTopic),
            sessionTransacted(sessionTransacted),
            brokerURI(brokerURI)
        {
        }

    virtual ~HelloWorldProducer()
    {
        cleanup();
    }

    void close()
    {
        this->cleanup();
    }

    virtual void run();

private:
    void cleanup();

};

#endif
