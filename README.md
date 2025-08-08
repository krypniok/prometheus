# Projektidee: Deuteros/Millennium/Supremacy Hybrid

## Ziel

Ein futuristisches Strategiespiel im Stil von *Deuteros*, *Millennium 2.2* und *Supremacy* – vollständig im Webbrowser spielbar per **WebGL**, mit **Grafiken**, **Sound**, und optional **Touch/Controller-Support**.

## Kernfeatures (erste Iteration)

* 🌍 **Planetenansicht** mit Ressourcenanzeige, Bauplänen, Bevölkerungsstand, Forschung & Infrastruktur
* 🚀 **Raumschiffbau** mit Modulen, Energieversorgung, Crew & Fracht
* 🔭 **Galaxiekarte** mit Sonnensystemen, Planeten, Handelsrouten, Kolonien
* 🧠 **Forschungssystem** mit verzweigtem Tech-Tree und experimentellen Entwicklungen
* ⛏️ **Rohstoffabbau & Produktion** auf Planeten und Monden
* 📦 **Lagerhaltung und Logistik** (z. B. Transportdrohnen, Hangars, Raumhäfen)
* 🧑‍🚀 **Crew-Management** mit Fähigkeiten, Loyalität, Gesundheit
* 🔊 **Retro-Soundtrack & Effekte**, idealerweise Synthwave/Space Ambient
* 🖼️ **Pixelgrafik-/Lowpoly-Stil**, inspiriert von Amiga/Atari-Ära

## Technologien

* 🧱 **WebGL/Three.js** für 3D-Visualisierung (Sternensysteme, Schiffe, Interfaces)
* 🎮 Eingabe via **Maus, Tastatur, Touch**, evtl. Controller
* 🎨 Assets: Spritesheets & Texturen (BMP/PNG), Audio (OGG/WAV)
* 🔧 Code in **JavaScript (ES6+)** mit Modulstruktur

## Projektphasen

1. **Prototyp UI** mit Platzhaltergrafiken:

   * Hauptmenü
   * Sternensystem mit Planeten
   * Ressourcenfenster + Produktionsliste

2. **Gameplay-Logik entwickeln:**

   * Echtzeit- vs. Rundenlogik entscheiden
   * Ressourcenkreisläufe simulieren
   * Ereignisse (Meteoriten, Rebellionen, KI-Angriffe)

3. **Grafik & Sound integrieren:**

   * GUI-Skins im Retro-Stil
   * Audio für Klicks, Musik, Ambient

4. **Speicher-/Ladefunktion** (lokal zuerst, dann optional WebSync)

5. **Polishing & Balancing**

---

## Noch zu klären:

* Welche Perspektive fürs Hauptspiel? (z. B. isometrisch? komplett 3D? Split-View?)
* Wie komplex soll KI-Interaktion werden? (diplomatisch, militärisch, wirtschaftlich)
* Sollen Raumkämpfe aktiv steuerbar sein oder automatisch?
* Wieviel Micromanagement vs. Automatisierung ist gewünscht?
* Wie „hardcore“ darf’s werden? Zielgruppe: nostalgische Retro-Fans oder breiter?

---

## Hinweis an Codex:

Wenn du das Projekt initialisieren sollst, beginne mit:

* einem Starter-Projekt in JavaScript mit WebGL-Canvas
* Dummy-Planetendarstellung (rotierende Kugel)
* einfachem UI (Sidebar mit Platzhalterwerten)
* Ladebarem assets/-Ordner (Texturen, Audio)
* README mit Setup-Anleitung für lokalen Webserver

### Du darfst bei fehlenden Angaben kreativ sein – aber **Fragen stellen**, wenn nötig!

> Projektname-Vorschlag: "**ExoGenesis**" oder "**Millenia Prime**"

Let’s build the space legacy! 🚀

## Lokal starten

Um den Entwicklungsserver zu starten:

```bash
npm install
npm run dev
```

Dies startet einen Vite-Server, der `index.html` unter http://localhost:5173 bereitstellt.
