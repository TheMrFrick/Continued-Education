package com.cs336.pkg;

public class User {
	private String username, password, type;
	
	public User() {
		
	}
	
	public String getUsername() {
		return this.username;
	}
	
	public void setUsername(String username) {
		this.username = username;
	}
	
	public String getPassword() {
		return this.password;
	}
	
	public void setPassword(String password) {
		this.password = password;
	}
	
	public String getType() {
		return this.type;
	}

	public void setType(String userType) {
		this.type = userType;
	}
	
}
