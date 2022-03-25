<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1" import="com.cs336.pkg.*"%>
<%@ page import="java.io.*,java.util.*,java.sql.*"%>
<%@ page import="javax.servlet.http.*,javax.servlet.*"%>
<!DOCTYPE html>
<html>
<head>
<%@include file="parts/meta.jsp" %>
<title>Search Results</title>
<%@include file="parts/header.jsp" %>
</head>
<body>
	<jsp:include page="parts/navbar.jsp"></jsp:include>
	
	<%
		String search = (String) request.getParameter("search");
	%>
	
	<div class = "contianer">
		<!--  get the items passed to this URL from the DB and showcase them in a table -->
		
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
			    ArrayList<String> words = new ArrayList<String>();
			    words.addAll(Arrays.asList(search.split(" ")));
			    if (!words.contains(result.getString("genderMake")) && !words.contains(result.getString("color"))
			            && !words.contains(result.getString("brand")) && !words.contains(result.getString("seller")))
			        continue;
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