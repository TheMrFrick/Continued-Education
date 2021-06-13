package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"

	"github.com/gorilla/mux"
)

//The struct that we will use to parse the json,
//assuming that the json is exactly how we want it
type IP struct {
	Address []string `json:"ips"`
}

//main method
func main() {
	//creating a mux router since it seems that Golang users prefer this over base packages
	router := mux.NewRouter().StrictSlash(true)

	//setting up the page /checkips and connecting a handler
	router.HandleFunc("/checkips", checkIps)

	//running locally.
	log.Fatal(http.ListenAndServe(":5000", router))
}

//CheckIps is the /checkips page handler.
//This will run all the items that should be handled for the /checkips page
//Params
// - w: http.ResponseWriter - base item needed for the framework. How we write to the webpage
// - r: pointer to http.Request - base item needed for the framework. How we can handle requests to the page
func checkIps(w http.ResponseWriter, r *http.Request) {
	//create the IPs object
	var ips IP

	//lets decode the json that we will get from the page.
	err := json.NewDecoder(r.Body).Decode(&ips)

	//error handle the decoding
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	//output on the page, or the curl the IP count.
	fmt.Fprintf(w, "IP Count: %d", len(ips.Address))
}
