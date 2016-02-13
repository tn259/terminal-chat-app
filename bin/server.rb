# server.rb
#
#Server has a TCPServer which accepts TCPSockets
# 
#
#require_relative '../lib/chat-client-handler'
require 'socket'

class Server
	#allow reading of ip and port as an exercise in testing for now
	attr_reader :ip
	attr_reader :port
	attr_reader :server
	attr_reader :message_queue
	attr_reader :clients
	def initialize(ip, port)
		@ip = ip
		@port = port
		@ip.freeze
		@port.freeze
		@server = TCPServer.new(@ip, @port)
	#When we utilise username and password use hash
		#@clients = Hash.new
	#For now use array
		@clients = Array.new  
	#	@message_queue = Queue.new
	end
#Listen for multiple clients and close connection after welcoming each one
	def run 
		loop {
			print "Awaiting new client..."	
			Thread.start(@server.accept) do |client|
			#	ChatClientHandler.new(client).run
				@clients << client
				listen_and_broadcast(client)	
			end
		}.join
	end
	
#Perhaps just listen_and_broadcast so no need for queue
#Because using array for clients we will be broadcasting to ourselves
	def listen_and_broadcast(client)
		loop {	
			message = client.gets
			@clients.each do |c|
				c.puts "New message ->"
				c.puts message
			end
		}		
	end
	
end

if __FILE__ == $0
	server = Server.new "127.0.0.1", 3000 
	server.run
end
