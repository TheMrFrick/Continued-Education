<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp" %>
<title>Create Forum</title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>
	
	<div class="container">
		<form action="insertForum.jsp" method="POST" class="form-inline">
			<fieldset>
				<legend>Title:</legend>
				<input type="text" name="title" value="title"><br>
			</fieldset>
			<fieldset>
				<legend>Question?</legend>
				<input type="text" name="quest" value="quest"><br>
			</fieldset>
			<input type="submit" value="Submit"/>

		</form>
	</div>
</body>
</html>