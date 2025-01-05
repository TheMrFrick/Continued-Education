import Axios from "axios";
import clipboard from "clipboardy";

// const clipboard = require('clipboardy');

async function obtainDeckUrl(deck_code: string) {
  try {
    const res = await Axios.get(
      `https://api.bandai-tcg-plus.com/api/user/deck/url_code?deck_code=${deck_code}`
    );
    // console.log(res.data);
    return res.data.success.url_code;
  } catch (error) {
    console.error(error);
    console.log('Unable to obtain url code');
    return false;
  }
}

async function getDeckData(deck_url: string) {
  try {
    const res = await Axios.get(
      `https://api.bandai-tcg-plus.com/api/user/deck/recipe?url_code=${deck_url}&game_title_id=1&encode=0&app_version=9.9.9`
    );
    // console.log(res.data);
    return res.data.success;
  } catch (error: any) {
    console.log(error.response);
    console.log('unable to obtain deck data');
    console.log(error.error);
    return false;
  }
}

function parseDeckData(data: any[]) {
  let running_info: any = {};

  for (const card of data) {
    running_info[card.card_number] = card.card_count;
  }

  return running_info;
}

async function main(deck_code: string) {
  let deck_url: string | false;
  let deck_data: any | false;

  let deck_info: { [key: string]: any } = {};
  // TCG+ hides the deck behind a deck_url
  deck_url = await obtainDeckUrl(deck_code);
  if (deck_url) {
    // console.log(deck_url);
    // Use the deck_url found to obtain the deck_data
    deck_data = await getDeckData(encodeURIComponent(deck_url));
  }
  if (deck_data) {
    deck_info.main = parseDeckData(deck_data.main_deck);
    deck_info.side = parseDeckData(deck_data.side_deck);
    deck_info.z = parseDeckData(deck_data.extra2_deck);
    deck_info.leader = parseDeckData(deck_data.extra_deck);
  }

  console.log(deck_info);
  clipboard.writeSync(JSON.stringify(deck_info));
}

main("T2KLzwUnsg0UTMRO");
