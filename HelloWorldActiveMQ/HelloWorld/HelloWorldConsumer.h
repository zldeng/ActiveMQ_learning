#ifndef _HELLO_WORLD_CONSUMER_H_
#define _HELLO_WORLD_CONSUMER_H_

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


class HelloWorldConsumer : public ExceptionListener,
	  public MessageListener,public Runnable
{
	private:
		CountDownLatch latch;
		CountDownLatch doneLatch;

		Connection* connection;
		Session* session;
		Destination* destination;
		MessageConsumer* consumer;

		long waitMillis;
		bool useTopic;
		bool sessionTransacted;
		std::string brokerURI;

	private:
		HelloWorldConsumer(const HelloWorldConsumer&);
		HelloWorldConsumer& operator=(const HelloWorldConsumer&);

	public:
		HelloWorldConsumer(const std::string& brokerURI,const int numMessages,
				const bool useTopic,const bool sessionTransacted = false,
				const int waitMillis = 30000):latch(1),doneLatch(numMessages),
		connection(NULL),session(NULL),destination(NULL),consumer(NULL),
		waitMillis(waitMillis),useTopic(useTopic),sessionTransacted(sessionTransacted),
		brokerURI(brokerURI)
	{
	}

		virtual ~HelloWorldConsumer()
		{
			cleanup();
		}

		void close()
		{
			cleanup();
		}

		void waitUntilReady()
		{
			latch.await();
		}

		virtual void run();
		virtual void onMessage(const Message* message);
		virtual void onException(const CMSException& ex AMQCPP_UNUSED);

	private:
		void cleanup();
};

#endif
