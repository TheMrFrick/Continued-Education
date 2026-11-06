const BASE = "https://palworldapi.azurewebsites.net";

async function getJson(path) {
  const res = await fetch(`${BASE}${path}`);
  if (!res.ok) {
    const body = await res.text().catch(() => "");
    throw new Error(`${res.status} ${path} ${body}`);
  }
  return res.json();
}

export async function fetchAllTribes() {
  const pageSize = 100;
  let page = 1;
  let totalPages = 1;
  const results = [];
  do {
    const data = await getJson(
      `/v1/pals?Pagination.PageNumber=${page}&Pagination.PageSize=${pageSize}`
    );
    results.push(...data.results);
    totalPages = data.pagination.totalPages;
    page++;
  } while (page <= totalPages);
  return results;
}

export async function breed(tribeNameA, tribeNameB) {
  const data = await getJson(
    `/v1/pals/breed?palNameA=${encodeURIComponent(tribeNameA)}&palNameB=${encodeURIComponent(tribeNameB)}`
  );
  return data.identity.tribeName;
}
