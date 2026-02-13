const express = require("express");
const { spawn } = require("child_process");
const path = require("path");

const app = express();
app.use(express.static(path.join(__dirname)));

app.get("/analyze", (req, res) => {
    const pass = req.query.pass;

    const process = spawn("./password", [pass]);

    let result = "";

    process.stdout.on("data", (data) => {
        result += data.toString();
    });

    process.on("close", () => {
        let score = parseInt(result);

        let strength = "";
        if (score <= 30) strength = "VERY WEAK";
        else if (score <= 50) strength = "WEAK";
        else if (score <= 75) strength = "MODERATE";
        else strength = "STRONG";

        res.json({ score, strength });
    });
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log("Server running...");
});
