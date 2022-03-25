<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp"%>
<title>Login Check Error</title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>

	<div class="container">
		<div style="text-align: center">
			<h3 style="color: red">You are not logged in.</h3>
			<h5>Please use the navbar above to log in.</h5>
		</div>
	</div>
</body>
</html>