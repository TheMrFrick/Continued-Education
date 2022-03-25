<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp"%>
<title>Add Bid</title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>

	<%
		String user = (String) session.getAttribute("user");
		if (user == null) {
			response.sendRedirect("notLoggedIn.jsp");
		}
	%>

	<%
		String auction = (String) request.getParameter("auctionID");
	%>

	<h2>Add Bid onto <%=auction %></h2>
	<br>
	<hr>
	<br>
	<form action="insertBid.jsp" method="GET" class="form-inline">
		<fieldset>
			<input type="text" readonly name="auctionID" value=<%=auction %>>
			<legend>Bid: </legend>
			<input type="number" name="bid"><br>
		</fieldset>
		<input type="submit" value="Submit" />
	</form>
</body>
</html>