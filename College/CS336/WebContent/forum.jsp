<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp"%>
<title>Forum</title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>

	<div class="container">
		<!-- This will go into the forum table and just list them out.  -->
		<!--  Users will go through a specific page for each forum -->
		
		<a href="createForum.jsp">
			<button class="button button1" value="createForum">New Forum</button>
		</a>
		
		<!--  List all the forum posts with a link to the forum  -->
		<!--  this website should help https://stackoverflow.com/questions/1890438/how-to-get-parameters-from-the-url-with-jsp -->
		
	</div>
</body>
</html>