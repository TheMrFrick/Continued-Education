<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1" import = "com.cs336.pkg.*"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Insert Item</title>
</head>
<body>
	<%@ page import="java.sql.*"%>
	<jsp:useBean id = "obj" class = "com.cs336.pkg.Item"/>
	
	<jsp:setProperty property="*" name="obj"/>
	
	<% 
		String owner = (String)session.getAttribute("user");
		
		boolean result = ApplicationDB.createItem(obj, owner);
		

		if(result){
			out.println("Item successfully added");
		} else{
			out.println("Failure: Improper input");
			%>
			
			<a href='addItem.jsp'>Try Again</a>
			
			<%
		}
	
	%>

</body>
</html>