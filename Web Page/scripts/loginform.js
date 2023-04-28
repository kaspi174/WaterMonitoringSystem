function login() {
  var username = document.getElementById("username").value;
  var password = document.getElementById("password").value;
  if (username === "myusername" && password === "mypassword") {
    alert("Login successful!");
    window.location.href = "/Web Page/directory/index.html";
  } else {
    alert("Login failed.");
  }
}
