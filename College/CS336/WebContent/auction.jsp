<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1" import = "com.cs336.pkg.*"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp" %>
<title>Auction <%= request.getParameter("name") %></title>
<%@include file="parts/header.jsp" %>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>
	
	<%@ page import="java.sql.*"%>
	
	
	<div class="container">
		<%
		String id = (String)request.getParameter("auctionID");

		ApplicationDB dao = new ApplicationDB();

		Connection connection = dao.getConnection();
		
		try{
			PreparedStatement ps = connection.prepareStatement("SELECT * FROM Auctions WHERE auctionId = ?");
			ps.setString(1, id);
		
			ResultSet rs = ps.executeQuery();
			while(rs.next()){
				
		%>
		<div style="align:center; display:block; border-style:double">
			<h2>AuctionID:<%=rs.getString("auctionID")%>	</h2>
			<hr> 	
			<br>	
			<p>ItemID:<%=rs.getString("itemID") %></p>
			<br> 
			<p><!-- current price --></p>
			<br>
			<p>Auction ends at:<%=rs.getString("endTime") %></p>
			<br>
		<% 
			}
			rs.close();
 		} 
		catch (SQLException e){
			e.printStackTrace();
		}
			%>
			<a href="addBid.jsp"><button class="button button1">Add Bid</button></a>
		</div>
	</div>
</body>
</html>