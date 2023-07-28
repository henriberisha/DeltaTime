# DeltaTime

This repository contains a short script written for Arduino platform that calculates a date-time from NOW when given the number of seconds passed

- I put a lot of comments to explain in detail what the code is doing
- Miliseconds are not handled, they will be zero
- Only future dates can be calculated, deltaSeconds >= 0
- Negative delta is not handled, cannot find past dates
- If any argument is inputed as a float, it will be casted to closest (floor) int
