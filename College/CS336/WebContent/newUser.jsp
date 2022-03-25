<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp"%>
<title>Create New User</title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>
	<div class="container">
		<h2>Create New User:</h2>
		<hr>
		<form action="createUser.jsp" method="POST" class="form-inline">
			<label for="username">Username:</label> <input type="text"
				name="username" placeholder="Enter Desired Username" /> <br> <label
				for="password">Password:</label> <input type="password"
				name="password" /> <br>
			<div>
				<label for="type">Type:</label><br> <input type="radio"
					name="type" value="Basic" checked> Basic User <br> <input
					type="radio" name="type" value="Representative"> Customer
				Representative <br>
			</div>
			<input type="submit" value="Create User">
		</form>
	</div>
</body>
</html>