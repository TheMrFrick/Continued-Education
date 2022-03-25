<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
	pageEncoding="ISO-8859-1" import="com.cs336.pkg.*"%>
<%@ page import="java.io.*,java.util.*,java.sql.*"%>
<%@ page import="javax.servlet.http.*,javax.servlet.*"%>
<!DOCTYPE html>
<html>
<head>

<%@include file="parts/meta.jsp"%>
<title>Browse</title>
<%@include file="parts/header.jsp"%>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>

	<a href="createAuction.jsp"><button class="button button1"
			value="createAuction">Make Auction</button></a>

	<div class="container">
		<!-- Fill this area in -->
		<!-- Area should include a connection to the database that will grab items -->
		<!--   Each item should be displayed. Maybe can split this up into different sections -->
		
		<table>
			<tr>
				<th>Gender Make</th>
				<th>Color</th>
				<th>Brand</th>
				<th>Seller</th>
				<th>End Time</th>
				<th>View</th>
			</tr>
		<%
		try {
	
			ApplicationDB db = new ApplicationDB();	
			Connection con = db.getConnection();		

			Statement stmt = con.createStatement();
			ResultSet result = stmt.executeQuery("SELECT Items.genderMake, Items.color, Items.brand, Auctions.auctionID, Auctions.seller, Auctions.endTime" +
			        " FROM Items INNER JOIN Auctions ON Auctions.itemID=Items.itemID");

			while (result.next()) {
		%>
			<tr>
				<td><%=result.getString("genderMake")%></td>
				<td><%=result.getString("color")%></td>
				<td><%=result.getString("brand")%></td>
				<td><%=result.getString("seller")%></td>
				<td><%=result.getString("endTime")%></td>
				<td><a href="auction.jsp?auctionID=<%=result.getString("auctionID")%>">View</a></td>
			</tr>
		<%
			}

			//close the connection.
			db.closeConnection(con);
		} catch (Exception e) {
			out.print(e);
		}
		%>
		</table>
	</div>
</body>
</html>