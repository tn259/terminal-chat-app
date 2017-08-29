# server.rb
#
# Server has a TCPServer which accepts TCPSockets
# Has basic username functionality so each client has a name
# Able to serialize and deserialize users
# Next password functionality included
# Will need to implement from client also
#

require 'socket'
require_relative '../lib/user'

class Server
	# allow reading of ip and port as an exercise in testing for now
	attr_reader :ip
	attr_reader :port
	attr_reader :server
	attr_reader :message_queue
	attr_reader :clients
	attr_reader :connections
	attr_reader :users
	attr_reader :file

	PWORDREGEX = /^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[^\w\s]).{8,}$/

	def initialize(ip, port)
		@ip = ip
		@port = port
		@ip.freeze
		@port.freeze
		@server = TCPServer.new(@ip, @port)
	# When we utilise username and password use hash
		@connections = Hash.new
		@clients = Hash.new 
		@connections[:clients] = @clients 
		@users = Array.new
		@file = '../users.dat'
	end

        # Listen for multiple clients and close connection after welcoming each one

	def run
		load_users
		loop {
			print "Awaiting new client..."	
			Thread.start(@server.accept) do |client|
			begin
				client.puts "Create new user account, y or n?"
				choice = client.gets.chomp
				create_account(client) if choice == 'y'
				account = nil
				loop {
					client.puts "Username:"
					username = client.gets.chomp
					username_found = false
					@users.each do |user|
                                        	if username == user.username
							username_found = true
							account = user
                                       		end
                                	end
					redo unless username_found
					already_logged_in = false
					@connections[:clients].each do |other_account, other_client|
						if account.username == other_account.username
							already_logged_in = true
							client.puts "This is already logged in"
							break
						end
					end
					redo if already_logged_in
					break
				}
				loop {
					client.puts "Password:"
					password = client.gets.chomp
					unless password == account.password
						client.puts "Password incorrect"
						redo 
					end	
					break
				}
			        # Check password is the same
				puts "#{account.username} has joined"
				@connections[:clients][account] = client
				client.puts "Thanks for joining the conversation!"
				listen_and_broadcast(account, client)
			rescue SocketError => e
				puts e.message
			end	
			end
		}.join
	end
	
	def create_account(client)
		username = ""
		loop {
			client.puts "Enter your desired username:"
			username = client.gets.chomp
			username_taken = false
			@users.each do |user|
				puts user.username
                                if username == user.username
                                	client.puts "This username is taken"
                                        username_taken = true
                                end
                        end
			redo if username_taken
			break
		}
		loop {
			client.puts "Create your password:"
			password = client.gets.chomp
			unless PWORDREGEX.match(password).nil?
			then 
				@users.push(User.new(username, password))	
				save_users
				client.puts "Account created"
				break
			else
				client.puts "Your password is invalid"
				redo
			end
		}		

	end

	# Perhaps just listen_and_broadcast so no need for queue
	def listen_and_broadcast(account, client)
		loop {
			message = client.gets
			if message == nil
				@clients.delete(account)
				puts "#{account.username} has left the conversation"
				break
			end
			@connections[:clients].each do |other_account, other_client|
				unless account.username == other_account.username
					other_client.puts "#{account.username}: #{message}"
				end
			end
		}	
		client.close	
	end

	# Serialize and deserialize users will refactor later
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
