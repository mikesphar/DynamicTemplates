# Changelog

## [Unreleased]

### Added
- Title page (`RenderArea/RenderArea--drawTitlePage.cpp`): a new `drawTitlePage()` function renders a full-page title leaf as the first page of every generated PDF, so that planner pages land on the correct sides when printing double-sided. The design uses the existing visual language: gradient bars, Blue Highway / Monaco fonts, and the standard grey palette. Content: a dark header bar labelled "DIY Planner", the planner year in large Monaco numerals (or "2026–27" style when the date range crosses a year boundary), a thin rule, the formatted date range, and a footer accent bar.
- "Include title page" checkbox (`MyWindow--createCalendarTab.cpp`): added to the Calendar tab, defaulted to checked. Unchecking it omits the title page from the PDF without affecting the on-screen preview.

### Fixed
- `MyWindow.cpp`: Default Line Width (10.0) and Line Spacing (Wide Ruled) were both silently ignored at startup. `createLinesTab()` emits their signals before `createDockWindows()` establishes the connections to `RenderArea`, so those initial values are dropped. Fixed by re-emitting both after the connections — `linesComboBoxChanged(0)` (Wide Ruled) and `emit penWidthChanged(100)` (10.0). Also changed the `linesComboBoxChanged` post-connection call from index 1 (College Ruled) to index 0 (Wide Ruled), and updated the matching `lineSpaceValue` initializer from `7.10` to `8.70`. `createDockWindows()` emits the initial line-spacing signal *after* establishing the signal/slot connections, so the call at the end of `createDockWindows()` — not the one inside `createLinesTab()` — determines the actual startup value. Changed that call from `linesComboBoxChanged(1)` (College Ruled) to `linesComboBoxChanged(0)` (Wide Ruled), and updated the matching `lineSpaceValue` initializer from `7.10` to `8.70`.
- `RenderArea/calendars/twoPagePerDay/twoPagePerDayDrawDay.cpp`: `justAbox.setBottom()` was setting an *absolute* Y coordinate (`howManyTall * displayLineSpacing`) that happened to be correct when `page.y() == 0` (tablet paper with no top margin) but fell short by the top-margin height when using Letter paper with 0.25 in margins. Fixed to `page.y() + howManyTall * displayLineSpacing`.
- `DynamicTemplates.pro`: Removed outdated `QMAKE_MAC_SDK` path and `CONFIG+= x86 ppc` targeting PowerPC/32-bit x86; modern Qt/Xcode now auto-detects the SDK and architecture (fixes build on Apple Silicon and Intel Macs)
- `RenderArea/RenderArea--drawBar.cpp`: Added missing `#include <QPainterPath>` required by newer macOS SDK (macOS 26 / Xcode) which no longer pulls it in transitively
- `RenderArea/RenderArea--drawGradientBox.cpp`: Replaced `QLinearGradient` brush with a banded solid-colour simulation (64 steps, clipped to bar shape). Qt's `QPrinter` PDF backend silently drops gradient brushes, causing gradient header bars to disappear entirely in saved PDFs. The banded approach renders identically on screen and in PDF output.
- `RenderArea/RenderArea--draw.cpp`: Qt's `QPrinter` creates PDF files with mode `0400` (read-only) on macOS, causing Preview and other editors to report the file as "locked". Added `QFile::setPermissions()` call after `painter->end()` to restore standard writable permissions (`rw-r--r--`). Also added a `removexattr` call (macOS only) to strip the `com.apple.quarantine` extended attribute that macOS stamps on files created by unsigned apps, which causes Preview to open PDFs in read-only mode.

### Fixed (compiler warnings)
- `RenderArea/RenderArea--draw.cpp`: Replaced deprecated `QPrinter::setPaperSize(QSizeF, DevicePixel)` with `setPageSize(QPageSize(..., Millimeter))`, converting from internal dot units using `printer->resolution()`. Removed unused `printerRect` variable (and the deprecated `paperRect()` call it came from).
- `RenderArea/RenderArea--fitRectToString.cpp`, `RenderArea/RenderArea--fitStringInRect.cpp`: Replaced deprecated `QFontMetrics::width()` with `horizontalAdvance()`.
- `MyWindow--createCalendarTab.cpp`: Removed unused `QDate now` variable in `spanComboChanged`.
- `MyWindow--createPageSizeTab.cpp`: Removed unused `theMargins` variable in `updateSize` (margins are emitted directly via `pageMarginsChanged` signal).
- `RenderArea/calendars/twoPagePerYear/twoPagePerYearDrawOneDay.cpp`: Removed unused `QColor tempColor` variable.

### Changed
- Default Line Spacing changed from College Ruled to Wide Ruled; default Line Width changed from 5.5 to 10.0
- Default Page Size changed from Statement to Letter; default margins changed from asymmetric (0/0/0.6/0 in) to 0.25 in on all sides. Also removed hardcoded reMarkable tablet size override (140×194 mm) that was silently overwriting the combo box selection on every launch.
- Default Template changed from One Page per Day / DIY3 Left Hand Style to Two Pages per Day / DIY3 Journal Left; default First Appointment Time changed from 8am / 10am (inconsistent) to 9am

### Added
- `diyp.app` to `.gitignore` (macOS app bundle build artifact)
