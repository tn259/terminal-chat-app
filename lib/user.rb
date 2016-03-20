class User
	attr_reader :username
	attr_reader :password
	def initialize(username)
		@username = username
		#not yet implemented passwords
		@password = ""
	end
end
