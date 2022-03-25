package com.cs336.pkg;

import java.sql.Date;
import java.sql.Time;

public class Auction {

	private int itemID;
	private double minPrice, initPrice, bidIncr;
	private String seller;
	private String date;
	private String time;
	
	
	public int getItemID() {
		return this.itemID;	
	}
	
	public double getMinPrice() {
		return this.minPrice;
	}
	
	public double getInitPrice() {
		return this.initPrice;
	}
	
	public double getBidIncr() {
		return this.bidIncr;
	}
	
	public String getSeller() {
		return this.seller;
	}
	
	public String getDate() {
		return this.date;
	}
	
	public String getTime() {
		return this.time;
	}
	
	public void setItemID(int itemID) {
		this.itemID = itemID;
	}
	
	public void setMinPrice(double minPrice) {
		this.minPrice = minPrice;
	}
	
	public void setIntiPrice(double initPrice) {
		this.initPrice = initPrice;
	}
	
	public void setBidIncr(double bidIncr) {
		this.bidIncr = bidIncr;
	}
	
	public void setSeller(String seller) {
		this.seller = seller;
	}
	
	public void setDate(String date) {
		this.date = date;
	}
	
	public void setTime(String time) {
		this.time = time;
	}
	
}
