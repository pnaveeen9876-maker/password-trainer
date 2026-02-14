const express = require("express");
const app = express();

// Serve static files (like index.html)
app.use(express.static(__dirname));

app.get("/analyze", (req, res) => {
    const pass = req.query.pass || "";

    let score = 0;
    let suggestions = [];

    // Length check
    if (pass.length >= 8) {
        score++;
    } else {
        suggestions.push("Use at least 8 characters");
    }

    // Capital letter check
    if (/[A-Z]/.test(pass)) {
        score++;
    } else {
        suggestions.push("Add at least one CAPITAL letter");
    }

    // Number check
    if (/[0-9]/.test(pass)) {
        score++;
    } else {
        suggestions.push("Include at least one number");
    }

    // Special character check
    if (/[^A-Za-z0-9]/.test(pass)) {
        score++;
    } else {
        suggestions.push("Add a special character (!@#$%^&*)");
    }

    // Determine strength
    let strength = "Weak";
    if (score === 3) strength = "Medium";
    if (score === 4) strength = "Strong";

    res.json({
        score: score,
        strength: strength,
        suggestions: suggestions
    });
});

// Use Render's dynamic port
const PORT = process.env.PORT || 3000;

app.listen(PORT, () => {
    console.log("Server running on port " + PORT);
});
