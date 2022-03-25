<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<title>Login Form</title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>
	<div class="container">
		<h2>Login:</h2>
		<hr>
		<form action="checkLoginDetails.jsp" method="POST" class="form-inline">
			<label for="username">Username: </label> <input type="text"
				name="username" placeholder="Enter Username"/> <br /> <label for="password">Password:</label>
			<input type="password" name="password"  placeholder="Enter Password"/> <br /> <input
				type="submit" value="Submit" />
		</form>

		
	</div>

</body>
</html>