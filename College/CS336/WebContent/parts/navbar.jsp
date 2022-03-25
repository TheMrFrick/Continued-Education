<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>

<%
	String user = (String) session.getAttribute("user");

	boolean loggedIn = false;

	if (user != null) {
		loggedIn = true;
	}

	if (loggedIn) {
%>
<ul class="navbar">
	<li><a href="index.jsp">Home</a></li>
	<!-- 		<li><a href="news.asp">News</a></li> -->
	<li><a href="browse.jsp">Browse</a></li>
	<li style="float: right"><a href="logout.jsp">Logout</a></li>
	<li style="float: right"><a href="user.jsp?user=<%=user%>">My Account</a>
	<li style="float: right"><a href="search.jsp">Search</a></li>
	<li style="float: right"><a href="addItem.jsp">add Item</a></li>
	<li style="float: right"><a href="createAuction.jsp">create
			Auction</a></li>
	<!-- 		<li><a href="about.asp">About</a></li> -->
</ul>
<li style="float: right">Logged in as: 
<%
	out.println(user);
%>
</li>
<%
	} else {
%>
<ul class="navbar">
	<li><a href="index.jsp">Home</a></li>
	<!-- 		<li><a href="news.asp">News</a></li> -->
	<li><a href="browse.jsp">Browse</a></li>
	<li style="float: right"><a href="newUser.jsp">Sign Up</a></li>
	<li style="float: right"><a href="login.jsp">Login</a></li>
	<li style="float: right"><a href="search.jsp">Search</a>
	<li style="float: right"><a href="addItem.jsp">add Item</a>
	<li style="float: right"><a href="createAuction.jsp">create
			Auction</a> <!-- 		<li><a href="about.asp">About</a></li> -->
</ul>
<%
	}
%>




