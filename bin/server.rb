# server.rb
#
#Server has a TCPServer which accepts TCPSockets
#Has basic username functionality so each client has a name
#Able to serialize and deserialize users
#Next password functionality included
#Will need to implement from client also
#
#require_relative '../lib/chat-client-handler'
require 'socket'
#require 'marshal'
require_relative '../lib/user'

class Server
	#allow reading of ip and port as an exercise in testing for now
	attr_reader :ip
	attr_reader :port
	attr_reader :server
	attr_reader :message_queue
	attr_reader :clients
	attr_reader :connections
	attr_reader :users
	attr_reader :file
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
		@users = Array.new
		@file = '../users.dat'
	end
#Listen for multiple clients and close connection after welcoming each one
	def run
		load_users
		loop {
			print "Awaiting new client..."	
			Thread.start(@server.accept) do |client|
			begin
				username = client.gets.chomp
				username_taken = false
			#Checking some users twice will refactor later
				@connections[:clients].each do |other_name, other_client|
					if username == other_name || client == other_client
						client.puts "This username is taken"
						username_taken = true
					end
				end
				@users.each do |user|
					puts user.username
                                        if username == user.username
                                                client.puts "This username is taken"
                                                username_taken = true
                                        end
                                end
				redo if username_taken
				puts "#{username} has joined"
				@users.push(User.new(username))
				@connections[:clients][username] = client
				save_users
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

#Serialize and deserialize users will refactor later
	def save_users
		serialized_users = Marshal.dump(@users)
		File.open(@file, 'wb') {|f| f.write(serialized_users)}
	end

	def load_users
		if File.file?(@file) 
		then  deserialized_users = nil
			unless File.zero?(@file) 
			then 
				File.open(@file, 'rb') {|f| deserialized_users = Marshal.load(f)}
				deserialized_users.each {|a| @users.push(a)}
			end
		else @file = File.new(@file, 'w+')
		end
	end
	
end

if __FILE__ == $0
	server = Server.new "127.0.0.1", 3000 
	server.run
end
