const url = "https://topdeckdiffusion.com/api/decks/view";

async function scrapeTopDeckDiffusion() {
  const data = await fetch(url, {
    method: "POST",
    body: JSON.stringify({
      iddeck: 1572,
      iduserConnecte: 0,
      iduserDeck: 168,
    }),
    headers: { "Content-Type": "application/json" },
  });
  console.log(data.status);
  //   const json = await data.json();
  //   console.log(json);
}

scrapeTopDeckDiffusion();
