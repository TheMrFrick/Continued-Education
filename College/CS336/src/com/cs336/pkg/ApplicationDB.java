package com.cs336.pkg;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class ApplicationDB {
	
	public ApplicationDB(){
		
	}

	/**
	 * Creates connection to SQL database
	 * @return connection to database
	 */
	public Connection getConnection(){
		
		//Create a connection string
		String connectionUrl = "jdbc:mysql://cs336-db.cyltgcwmeu5o.us-east-2.rds.amazonaws.com:3306/FinalProject";
		Connection connection = null;
		
		try {
			//Load JDBC driver - the interface standardizing the connection procedure. Look at WEB-INF\lib for a mysql connector jar file, otherwise it fails.
			Class.forName("com.mysql.jdbc.Driver").newInstance();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		try {
			//Create a connection to your DB
			connection = DriverManager.getConnection(connectionUrl, "admin", "adminpass");
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return connection;
		
	}
	
	public void closeConnection(Connection connection){
		try {
			connection.close();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	
	public static void main(String[] args) {
		ApplicationDB dao = new ApplicationDB();
		Connection connection = dao.getConnection();
		
		
		System.out.println(connection);		
		dao.closeConnection(connection);
	}
	/**
	 * Checks SQL database for username and password
	 * @param user
	 * @return
	 */
	public static boolean log(User user) {
		ApplicationDB dao = new ApplicationDB();
		boolean result = false;
		
		Connection connection = dao.getConnection();

		try {
			PreparedStatement ps = connection.prepareStatement("SELECT * FROM Users WHERE username=? and pass=?");
			
			ps.setString(1, user.getUsername());
			ps.setString(2, user.getPassword());
			
			ResultSet rs = ps.executeQuery();
			result = rs.next();
			dao.closeConnection(connection);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	
		return result;
	}
	
	public static boolean createItem(Item item, String user) {
		ApplicationDB dao = new ApplicationDB();
		boolean result = false;
		
		Connection connection = dao.getConnection();
		
		try {
			PreparedStatement mainPS = connection.prepareStatement("INSERT INTO Items(genderMake, color, brand, material, owner) VALUES (?, ?, ?, ?, ?)");
			
			mainPS.setString(1, item.getGender());
			mainPS.setString(2, item.getColor());
			mainPS.setString(3, item.getBrand());
			mainPS.setString(4, item.getMaterial());
			mainPS.setString(5, user);
			
			
			PreparedStatement subPS = null; 
			
			if(item.getType().equalsIgnoreCase("top")){
				subPS = connection.prepareStatement("INSERT INTO Item_Tops(topSize,topStyle) VALUES (?,?)");
			}
			if(item.getType().equalsIgnoreCase("bottom")){
				subPS = connection.prepareStatement("INSERT INTO Item_Bottoms(bottomSize, bottomStyle) VALUES (?,?)");
			}
			if(item.getType().equalsIgnoreCase("shoe")){
				subPS = connection.prepareStatement("INSERT INTO Item_Shoes(shoeSize, shoeStyle) VALUES (?,?)");
			}
			
			subPS.setString(1, item.getSize());
			subPS.setString(2, item.getStyle());
			
			
			int y = subPS.executeUpdate();
//			int x = mainPS.executeUpdate();
			
			if(y != 0) {
				result = true;
			}
			dao.closeConnection(connection);
			
		}catch (SQLException e) {
			e.printStackTrace();
		}
		
		return result;
		
	}
	
	public static boolean createUser(User user) {
		ApplicationDB dao = new ApplicationDB();
		boolean result = false;
		
		Connection connection = dao.getConnection();
		
		
		try {
			 //insert for the generic user instance
			 PreparedStatement ps = connection.prepareStatement("INSERT INTO Users(username,pass) VALUES (?,?)");
			 
			 ps.setString(1, user.getUsername());
			 ps.setString(2, user.getPassword());

			 int x = 0, y = 0;
			 PreparedStatement subPS = null;
			 
			if(user.getType().equalsIgnoreCase("Basic")) {
				 subPS = connection.prepareStatement("INSERT INTO User_EndUsers(username) VALUES (?)");
				 
				 subPS.setString(1, user.getUsername());
			 }
			
			if(user.getType().equalsIgnoreCase("Representative")) {
				subPS = connection.prepareStatement("INSERT INTO User_CustReps(username) VALUES (?)");
				 
				 subPS.setString(1, user.getUsername());
				
			}
			
			 
			 x = ps.executeUpdate();
			 y = subPS.executeUpdate();
			 if(x != 0 && y != 0) {
				 return true;
			 }
		 } catch(SQLException e){
			 e.printStackTrace();
		 } 
		
		
		dao.closeConnection(connection);
		return result;
		
	}
	
	public static boolean deleteUser(String username) {
		ApplicationDB dao = new ApplicationDB();
		boolean result = false;

		Connection connection = dao.getConnection();

		int x = 0;

		try {
			PreparedStatement ps = connection.prepareStatement("DELETE FROM Users WHERE username=(?)");
			ps.setString(1, username);

			x = ps.executeUpdate();
			if (x != 0) {
				result = true;
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return result;
	}
	
	 public static boolean createAuction(Auction auction, int item, String seller) {
		ApplicationDB dao = new ApplicationDB();
		boolean result = false;
		
		Connection connection = dao.getConnection();
		
		int x = 0;
		
		try {
			PreparedStatement ps = connection.prepareStatement("INSERT INTO Auctions(itemID, minPrice, initPrice, bidIncr, seller, endTime) VALUES(?, ?, ?, ?, ?, TIMESTAMP(?, ?))");
			
			ps.setInt(1, item);
			ps.setDouble(2, auction.getMinPrice());
			ps.setDouble(3, auction.getInitPrice());
			ps.setDouble(4, auction.getBidIncr());
			ps.setString(5, seller);
			ps.setString(6, auction.getDate());
			ps.setString(7, auction.getTime());
			
			x = ps.executeUpdate();
			if(x != 0) {
				result = true;
			}
			
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		
		return result;
		
		
	}
	

}
