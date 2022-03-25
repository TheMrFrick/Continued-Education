<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1" import="com.cs336.pkg.*"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Check Details</title>
</head>
<body>
	<%@ page import="java.sql.*"%>
	<jsp:useBean id="obj" class="com.cs336.pkg.User" />

	<jsp:setProperty property="*" name="obj" />
	<%
		String dest= "";
		if(obj.getUsername().equalsIgnoreCase("admin") && obj.getPassword().equalsIgnoreCase("adminpass")){
			session.setAttribute("user", "admin");
			
			dest = "admin.jsp";
		} else if(obj.getUsername().equalsIgnoreCase("admin")){
			out.println("Cannot use username admin, please try again");
			dest = "login.jsp";
		}
		
		boolean result = ApplicationDB.log(obj);

		if (result && !obj.getUsername().equalsIgnoreCase("admin")) {
			out.println("Success");
			dest = "index.jsp";
			session.setAttribute("user", obj.getUsername());
			out.println("welcome " + obj.getUsername());
			
			
			out.println("<a href='logout.jsp'>Log out</a>");
			
			//response.sendRedirect("index.jsp");

		} else {
			out.println("Failure");
			out.println("Invalid password <a href='login.jsp'>try again</a>");

		}
		response.sendRedirect(dest);
	%>
</body>
</html>