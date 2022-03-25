<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1" import = "com.cs336.pkg.*"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp" %>
<title>Insert Auction</title>
<%@include file="parts/header.jsp" %>
</head>
<body>
	<%@ page import="java.sql.*"%>
	<jsp:include page="parts/navbar.jsp"></jsp:include>

	<jsp:useBean id = "obj" class = "com.cs336.pkg.Auction"/>
	
	<jsp:setProperty property="*" name="obj"/>

	<% 
		int item = Integer.parseInt(request.getParameter("itemID"));
		String seller = (String)session.getAttribute("user");		
	
	
		boolean result = ApplicationDB.createAuction(obj, item, seller);
	
		if(result){

			response.sendRedirect("browse.jsp");
		} else{
			out.println("Failure: Improper input");
				
			%>	
			<a href='createAuction.jsp'>Try Again</a>
			<%
			
		}
		
		%>
		

	<div class="contianer">
		<!--  same as insertItem.jsp really -->
	</div>
</body>
</html>