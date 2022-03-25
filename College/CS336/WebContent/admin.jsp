<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"  import = "com.cs336.pkg.*"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp"%>
<title>Admin</title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<%@ page import="java.sql.*"%>
	<!-- check to see if the current user is the admin -->
	<!--  if they are, then they can continue onto the page -->
	<!--  if not, then they will be redirected -->
	
	<jsp:include page="parts/navbar.jsp"></jsp:include>
	<% 
	if(!session.getAttribute("user").equals("admin")){
		out.println("User is not admin. Being redirected to the home page");
		response.sendRedirect("index.jsp");
	}
	%>

	<div class="container">
		<h2>Users</h2>
		<!-- This area will hold all the users except the admin -->
		<!--  will be in a table and the table will have the option to edit the user -->
		<!--  and delete the user -->
		
		
		
		<table>
			<tr>
				<th>Username</th>
				<th>password</th>
				<th>Edit</th>
				<th>Delete</th>
			</tr>
		</table>
		<%
		//I think this goes here
		ApplicationDB dao = new ApplicationDB();
		
		Connection connection = dao.getConnection();
		
		try {
			PreparedStatement ps = connection.prepareStatement("SELECT * FROM Users WHERE NOT username = 'admin'");
			
			
			ResultSet rs = ps.executeQuery();
			while(rs.next()){
				%>
				 	<tr>
				 		<td><%=rs.getString("username") %></td>
				 		<td><%=rs.getString("pass") %></td>
				 	<tr>
				<% 
			}
			rs.close();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		dao.closeConnection(connection);
		%>
		
	</div>
</body>
</html>