import requests
import json
import time
import os
from pathlib import Path
from dotenv import load_dotenv

# Load .env file
env_path = Path(__file__).parent / ".env"
load_dotenv(env_path)

# Danh sách các query để fetch issues
QUERIES = [
    "repo:llvm/llvm-project is:issue label:llvm:optimizations type:Bug",
    "repo:llvm/llvm-project is:issue label:clang:frontend type:Bug",
    "repo:llvm/llvm-project is:issue label:backend:RISC-V type:Bug",
    "repo:llvm/llvm-project is:issue label:miscompilation type:Bug",
    "repo:llvm/llvm-project is:issue label:llvm:codegen type:Bug",
    "repo:llvm/llvm-project is:issue label:ABI type:Bug",
    "repo:llvm/llvm-project is:issue label:crash type:Bug",
]

GITHUB_TOKEN = os.getenv("GITHUB_TOKEN", "")
API_URL = "https://api.github.com/search/issues"
OUTPUT_FILE = r"bug.json"

def fetch_issues_from_query(query):
    """Fetch tất cả issues từ một query."""
    issues = []
    page = 1

    print(f"\n📌 Fetching issues for query: {query[:80]}...")

    while True:
        print(f"   - Fetching page {page}...")

        headers = {"Accept": "application/vnd.github.v3+json"}
        if GITHUB_TOKEN:
            headers["Authorization"] = f"token {GITHUB_TOKEN}"

        params = {"q": query, "per_page": 100, "page": page}
        response = requests.get(API_URL, params=params, headers=headers)

        if response.status_code != 200:
            print(f"   ❌ Error {response.status_code}: {response.text}")
            break

        data = response.json()
        items = data.get("items", [])

        if not items:
            print(f"   ✓ Hết kết quả (page {page})")
            break

        print(f"   ✓ Tìm thấy {len(items)} issues")

        for item in items:
            issues.append({
                "title": item.get("title", ""),
                "url": item.get("html_url", ""),
                "number": item.get("number", 0),
                "query": query
            })

        if len(items) < 100:
            break

        page += 1
        time.sleep(2)  # Tránh rate limit

    return issues

def main():
    all_issues = []

    print("=" * 60)
    print("LLVM Issues Fetcher")
    print("=" * 60)

    if not QUERIES:
        print("❌ Danh sách QUERIES trống. Vui lòng thêm query.")
        return

    # Fetch issues từ mỗi query
    for query in QUERIES:
        issues = fetch_issues_from_query(query)
        all_issues.extend(issues)
        print(f"   Tổng từ query này: {len(issues)} issues\n")

    # Xóa duplicate dựa trên Issue ID (number)
    unique_issues = {}
    for issue in all_issues:
        issue_id = issue["number"]
        if issue_id not in unique_issues:
            unique_issues[issue_id] = issue

    all_issues = list(unique_issues.values())

    # Lưu kết quả
    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        json.dump(all_issues, f, ensure_ascii=False, indent=4)

    print("=" * 60)
    print(f"✅ Đã lưu {len(all_issues)} issues (sau loại bỏ duplicate)")
    print(f"📁 File: {OUTPUT_FILE}")
    print("=" * 60)

if __name__ == "__main__":
    main()
