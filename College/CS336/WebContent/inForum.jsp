<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp"%>
<title>Forum <%=request.getParameter("ID")%></title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>

	<div class="container">
		<!-- go through the db and get the forum with the id ofrequest.getParameter("id")d") %> -->
		<!--  this should be able to give you all the information needed for the page -->
		<div style="align: center; display: block; border-style: double">
			<h2>
				<!-- name goes in here -->
			</h2>
			<hr>
			<br>
			<!--  go through all the questions and answers of the thread -->
			<!--  this could be the alerts and or emails in our db -->
		</div>
	</div>
</body>
</html>