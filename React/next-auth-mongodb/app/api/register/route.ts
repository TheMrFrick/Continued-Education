import { NextResponse } from "next/server";

export async function POST(req) {
    try {
        const {name, email, password} = await req.json();

        console.log("Name:", name);
        console.log("Email:", email);
        console.log("Password:", password);

        return NextResponse.json({message: "User Registered."}, { status: 201 });
    } catch (err) {
        return NextResponse.json({message: "Error occurred while registering the user." }, { status: 500 });
    }
}