import { connectMongoDB } from "@/lib/mongodb";
import User from "@/models/user";
import { NextResponse } from "next/server";
import bcrypt from 'bcryptjs';

export async function POST(req: any) {
    try {
        const {name, email, password} = await req.json();
        const hashedPassword = await bcrypt.hash(password, 10);
        await connectMongoDB();
        await User.create({name, email, password: hashedPassword});

        return NextResponse.json({message: "User Registered."}, { status: 201 });
    } catch (err) {
        console.log(err);
        return NextResponse.json({message: "Error occurred while registering the user." }, { status: 500 });
    }
}