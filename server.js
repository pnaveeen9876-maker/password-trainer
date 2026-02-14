const express = require("express");
const app = express();

app.use(express.static(__dirname));

app.get("/analyze", (req, res) => {
    const pass = req.query.pass;

    let score = 0;
    if (pass.length >= 8) score++;
    if (/[A-Z]/.test(pass)) score++;
    if (/[0-9]/.test(pass)) score++;
    if (/[^A-Za-z0-9]/.test(pass)) score++;

    let strength = "Weak";
    if (score >= 3) strength = "Medium";
    if (score >= 4) strength = "Strong";

    res.json({ score, strength });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log("Server running on port " + PORT);
});
