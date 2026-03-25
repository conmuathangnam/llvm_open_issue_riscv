import os
import json
import re
import urllib.parse
from pathlib import Path
import time
import requests
from datetime import datetime
from dotenv import load_dotenv

# Load từ .env file
load_dotenv()

# Cấu hình
INPUT_FILE = r"bug.json"
OUTPUT_DIR = r"output"
GITHUB_TOKEN = os.getenv("GITHUB_TOKEN", "")  # Đọc từ file .env hoặc env variable
CUT_OFF_DATE = datetime(2024, 9, 17)
# Regex tìm kiếm link attachment của github
ATTACHMENT_PATTERN = re.compile(
    r"(https://github\.com/(?:[\w-]+)/[\w-]+/assets/\d+/[\w-]+|"
    r"https://github\.com/user-attachments/assets/[\w-]+|"
    r"https://github\.com/user-attachments/files/\d+/[^\s\)\"']+)"
)

def get_issue_number(url):
    return url.rstrip("/").split("/")[-1]

def make_request(url, retries=3):
    headers = {"Accept": "application/vnd.github.v3+json"}
    if GITHUB_TOKEN:
        headers["Authorization"] = f"token {GITHUB_TOKEN}"
    
    for attempt in range(retries):
        try:
            response = requests.get(url, headers=headers, timeout=15)
            
            # Xử lý rate limit
            if response.status_code == 403 and "rate limit exceeded" in response.text.lower():
                reset_time = int(response.headers.get("X-RateLimit-Reset", time.time() + 60))
                sleep_time = reset_time - int(time.time()) + 5
                print(f"\n[!] Rate limit exceeded. Sleeping for {sleep_time} seconds...")
                time.sleep(max(sleep_time, 5))
                continue # Thử lại sau khi đợi
                
            if response.status_code == 200:
                return response.json()
            else:
                print(f"Error {response.status_code} fetching {url}: {response.text}")
                return None
        except requests.exceptions.RequestException as e:
            print(f"Lỗi truy cập {url}: {e} (thử lần {attempt + 1}/{retries})")
            time.sleep(2)
            
    print(f"Thất bại lấy dữ liệu từ {url} sau {retries} lần thử.")
    return None

def get_filename_from_url(url):
    parsed = urllib.parse.urlparse(url)
    basename = os.path.basename(parsed.path)
    if not basename:
        basename = "attachment_file"
    return basename

def download_file(url, target_path_base):
    try:
        # Check file size with HEAD request first
        head_response = requests.head(url, allow_redirects=True, timeout=15)
        if head_response.status_code == 200:
            content_length = head_response.headers.get("content-length")
            if content_length:
                file_size = int(content_length)
                max_size = 200 * 1024  # 200 KB
                if file_size > max_size:
                    size_kb = file_size / 1024
                    print(f"    - Bỏ qua: {get_filename_from_url(url)} ({size_kb:.1f} KB > 200 KB)")
                    return

        response = requests.get(url, stream=True, allow_redirects=True, timeout=15)
        if response.status_code == 200:
            # Try to get filename from Content-Disposition
            cd = response.headers.get("content-disposition")
            filename = ""
            if cd:
                fname = re.findall("filename=(.+)", cd)
                if len(fname) > 0:
                    filename = fname[0].strip('\"\'')

            if not filename:
                filename = get_filename_from_url(url)

            # URL safety decode
            filename = urllib.parse.unquote(filename)
            target_path = os.path.join(target_path_base, filename)

            # Tránh trùng tên
            counter = 1
            while os.path.exists(target_path):
                name, ext = os.path.splitext(filename)
                target_path = os.path.join(target_path_base, f"{name}_{counter}{ext}")
                counter += 1

            with open(target_path, 'wb') as f:
                for chunk in response.iter_content(chunk_size=8192):
                    f.write(chunk)
            print(f"    - Tải xong: {os.path.basename(target_path)}")
        else:
            print(f"    - Lỗi tải file {url}: HTTP {response.status_code}")
    except Exception as e:
        print(f"    - Lỗi tải file {url}: {e}")

def process_issue(issue_url):
    issue_number = get_issue_number(issue_url)

    issue_folder = os.path.join(OUTPUT_DIR, issue_number)
    md_path = os.path.join(issue_folder, "discussion.md")

    if os.path.exists(md_path):
        print(f"\nBỏ qua issue #{issue_number} do đã tải thành công trước đó.")
        return

    print(f"\nĐang xử lý issue #{issue_number}")
    os.makedirs(issue_folder, exist_ok=True)

    api_url = f"https://api.github.com/repos/llvm/llvm-project/issues/{issue_number}"
    issue_data = make_request(api_url)

    if not issue_data:
        print(f"Bỏ qua issue #{issue_number} do lỗi API.")
        return

    # Kiểm tra điều kiện: chỉ fetch issue đang open hoặc closed sau 17-09-2024
    state = issue_data.get("state", "")
    closed_at = issue_data.get("closed_at")

    if state == "closed" and closed_at:
        closed_date = datetime.fromisoformat(closed_at.replace("Z", "+00:00"))
        
        if closed_date.replace(tzinfo=None) < CUT_OFF_DATE:
            print(f"Bỏ qua issue #{issue_number} do đã close trước 17-09-2024.")
            return
    elif state != "open":
        print(f"Bỏ qua issue #{issue_number} do trạng thái không hợp lệ.")
        return

    # Lấy comments
    comments_url = issue_data.get("comments_url")
    comments_data = []
    if comments_url:
        comments_data = make_request(comments_url) or []

    # Gom nội dung
    title = issue_data.get("title", "")
    body = issue_data.get("body") or ""
    author = issue_data.get("user", {}).get("login", "Unknown")

    # Lấy labels, status, closed date
    labels = issue_data.get("labels", [])
    label_names = [label["name"] for label in labels] if labels else []
    status = "Open" if state == "open" else "Closed"
    closed_date_str = ""
    if closed_at:
        closed_date_str = f"\n**Closed Date:** {closed_at}"

    labels_str = ""
    if label_names:
        labels_str = f"\n**Labels:** {', '.join(label_names)}"

    md_content = f"# {title}\n\n**Author:** {author}\n**URL:** {issue_url}\n**Status:** {status}{labels_str}{closed_date_str}\n\n## Body\n\n{body}\n\n"
    all_text_for_attachments = body + "\n"
    
    if comments_data:
        md_content += "## Comments\n\n"
        for i, comment in enumerate(comments_data, 1):
            c_author = comment.get("user", {}).get("login", "Unknown")
            c_body = comment.get("body") or ""
            md_content += f"### Comment {i} - {c_author}\n\n{c_body}\n\n---\n\n"
            all_text_for_attachments += c_body + "\n"

    # Lưu nội dung
    md_path = os.path.join(issue_folder, "discussion.md")
    with open(md_path, "w", encoding="utf-8") as f:
        f.write(md_content)
    
    # Tìm file đính kèm
    attachments = ATTACHMENT_PATTERN.findall(all_text_for_attachments)
    # Loại bỏ duplicate
    attachments = list(set(attachments))
    if attachments:
        print(f"  * Tìm thấy {len(attachments)} file đính kèm.")
        for att_url in attachments:
            download_file(att_url, issue_folder)

def main():
    if not os.path.exists(INPUT_FILE):
        print(f"Không tìm thấy file {INPUT_FILE}")
        return
        
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    
    if not GITHUB_TOKEN:
        print("CẢNH BÁO: Chưa cấu hình biến môi trường GITHUB_TOKEN.")
        print("GitHub API giới hạn 60 requests/giờ (không có token). Quá trình có thể bị gián đoạn.")
        print("Bạn có thể cấu hình token trên command line: $env:GITHUB_TOKEN='your_token'")
        print("Tiếp tục chạy sau 3 giây...")
        time.sleep(3)
        
    with open(INPUT_FILE, "r", encoding="utf-8") as f:
        issues = json.load(f)
        
    print(f"Bắt đầu xử lý {len(issues)} issues...")
    for issue in issues:
        issue_url = issue["url"]
        process_issue(issue_url)
        time.sleep(0.5) # Chờ 0.5s để tránh spam nhanh
        

if __name__ == "__main__":
    main()
