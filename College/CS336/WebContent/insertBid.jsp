<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1" import="com.cs336.pkg.*"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp" %>
<title>Insert Bid</title>
<%@include file="parts/header.jsp" %>
</head>
<body>

<jsp:include page="parts/navbar.jsp"></jsp:include>
	<%@ page import="java.sql.*"%>
<!-- Must create a bidhistory -->
<!--  must create a bid -->
<!--  must connect them to auction -->

<%
	ApplicationDB dao = new ApplicationDB();
	String s = (String)request.getParameter("bid");
	double d = Double.parseDouble(s);
	ApplicationDB.createBid(d);
	
	String username = ((String) request.getParameter("user"));

	//I think this goes here
	

	Connection connection = dao.getConnection();

	try {
		PreparedStatement ps = connection
				.prepareStatement("SELECT * FROM Bids");

		ResultSet rs = ps.executeQuery();
		rs.last();
		ApplicationDB.createBidHistory(rs.getString("bidID"), request.getParameter("auctionID"));
		rs.close();
	}catch(SQLException e){
		
	}
	
	response.sendRedirect("auction.jsp?auctionID=" + request.getParameter("auctionID"));
%>

	
</body>
</html>