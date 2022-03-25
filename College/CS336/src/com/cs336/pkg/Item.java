package com.cs336.pkg;


public class Item {
	private String gender, color, material, brand, type, size, style;
	
	//because itemId is on autoincrement I don't think I need to determine it in the insertion 
	
	public String getGender() {
		return this.gender;
	}
	
	public String getColor() {
		return this.color;
	} 
	
	public String getMaterial() {
		return this.material;
	}
	
	public String getBrand() {
		return this.brand;
	}
	
	public String getType() {
		return this.type;
	}
	
	public String getSize() {
		return this.size;
	}
	
	public String getStyle() {
		return this.style;
	}
	
	public void setGender(String gender) {
		this.gender = gender;
	}
	
	
	public void setColor(String color) {
		this.color = color;
	}
	
	public void setMaterial(String material) {
		this.material = material;
	}
	
	public void setBrand(String brand) {
		this.brand = brand;
	}
	
	public void setType(String type) {
		this.type = type;
	}
	
	public void setSize(String size) {
		this.size = size;
	}
	
	public void setStyle(String style) {
		this.style = style;
	}
	
	
}

