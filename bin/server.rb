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
	attr_reader :messageQueue
	attr_reader :clients
	def initialize(ip, port)
		@ip = ip
		@port = port
		@ip.freeze
		@port.freeze
		@server = TCPServer.new(@ip, @port)
		@clients = Hash.new
		@messageQueue = Queue.new
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
	
	def get_message(client)
		message = client.gets
		@messageQueue.push message
	end

end

if __FILE__ == $0
	server = Server.new "127.0.0.1", 3000 
	server.run
end
