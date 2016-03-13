# server.rb
#
#Server has a TCPServer which accepts TCPSockets
#Has basic username functionality so each client has a name
#Next have the ability to serialise/deserialise the usernames so that the .txt file can act as a db of users 
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
	attr_reader :connections
	def initialize(ip, port)
		@ip = ip
		@port = port
		@ip.freeze
		@port.freeze
		@server = TCPServer.new(@ip, @port)
	#When we utilise username and password use hash
		@connections = Hash.new
		@clients = Hash.new 
		@connections[:clients] = @clients 
	#	@message_queue = Queue.new
	end
#Listen for multiple clients and close connection after welcoming each one
	def run 
		loop {
			print "Awaiting new client..."	
			Thread.start(@server.accept) do |client|
			begin
				username = client.gets.chomp
				username_taken = false
				@connections[:clients].each do |other_name, other_client|
					if username == other_name || client == other_client
						client.puts "This username is taken"
						username_taken = true
					end
				end
				redo if username_taken
				puts "#{username} has joined"
				@connections[:clients][username] = client
				client.puts "Thanks for joining the conversation!"
				listen_and_broadcast(username, client)
			rescue SocketError => e
				puts e.message
			end	
			end
		}.join
	end
	
#Perhaps just listen_and_broadcast so no need for queue
	def listen_and_broadcast(username, client)
		loop {
			message = client.gets
			if message == nil
				@clients.delete(username)
				puts "#{username} has left the conversation"
				break
			end
			@connections[:clients].each do |other_name, other_client|
				unless username == other_name
					other_client.puts "#{username}: #{message}"
				end
			end
		}	
		client.close	
	end
	
end

if __FILE__ == $0
	server = Server.new "127.0.0.1", 3000 
	server.run
end
