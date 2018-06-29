cc_binary(
	name = 'helloActiveMQ',
	srcs = [
		'HelloWorldConsumer.cpp',
		'HelloWorldProducer.cpp',
		'main.cpp'
	],
	deps = [
		'//ThirdParty/activemq-cpp-3.8.2:activemq-cpp'
	],
	optimize = ['-std=c++11',],
	dynamic_link = True
)
