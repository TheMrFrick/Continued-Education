<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp" %>
<title>Search</title>
<%@include file="parts/header.jsp" %>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>
	<div class="container">
		<form action="results.jsp" method="GET" class="form-inline">
			<input type="text" name="search" placeholder="search..."><br>
			<input type="submit" value="Submit">
		</form>
	</div>
	<!-- should basically get a query going. That query will be injected into another page -->
	
</body>
</html>