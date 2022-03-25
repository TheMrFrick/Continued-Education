<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1" import="com.cs336.pkg.*"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Create User</title>
</head>
<body>
	<%@ page import="java.sql.*" %>
	<jsp:useBean id = "obj" class = "com.cs336.pkg.User"/>
	
	<jsp:setProperty property="*" name="obj"/>
	
	<% 
		boolean result = ApplicationDB.createUser(obj);
		request.getParameter("type");
	
	if(result){
		out.println("User successfully created. To sign in, proceed to the ");
		
		%>
		<a href='login.jsp'> login page.</a>
		
		<% 
		
	} else{
		out.println("Failure: Improper input");
		%>
		
		<a href='newUser.jsp'>Try Again.</a>
		
		<%
	}

	
	%>
</body>
</html>