# server.rb
#
#Server has a TCPServer which accepts TCPSockets
# 
#
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
		@clients = Hash.new
		@message_queue = Queue.new
	end
#Listen for multiple clients and close connection after welcoming each one
	def run 
	loop {
		print "Awaiting new client..."	
		Thread.start(@server.accept) do |client|
			begin
				client.puts "Welcome to the Terminal-Chat-App"
				#client.puts "I'm afraid I'll have to say by for now"
				client.close
			rescue	SocketError =>  e
				puts e.message
			end	
		end
	}
	end
	
#Perhaps just listen_and_broadcast so no need for queue
	#def listen_and_broadcast(client)
	#	loop {	
	#		message = client.gets
	#		@clients.each do |c|
	#			c.puts message
	#		end
	#	}		
	#end
	
	def get_message(client)
		message = client.gets
		@message_queue.push message
	end

	def broadcast_message
		message = @message_queue.pop
		@clients.each do |client|
			client.puts message
		end
	end

end

if __FILE__ == $0
	server = Server.new "127.0.0.1", 3000 
	server.run
end
