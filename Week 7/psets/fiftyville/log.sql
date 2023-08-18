-- Keep a log of any SQL queries you execute as you solve the mystery.

-- checking description of crime
SELECT description FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street = 'Humphrey Street';

-- theft at 10:15am, checking interviews
SELECT * FROM interviews
WHERE day = 28 AND month = 7;

-- 10 minute after theft, thief left the bakery parking. Checking the bakery security logs
SELECT license_plate FROM bakery_security_logs
WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit';

-- looking for the people that were leaving the parking
SELECT * FROM people
WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
);

-- checking if someone called when leaving
SELECT * FROM phone_calls
WHERE caller IN (
    SELECT phone_number FROM people
    WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
)
) AND month = 7 AND day = 28

-- checking the first flight leaving fiftyville next day
SELECT * FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
(
    SELECT id FROM airports
    WHERE full_name LIKE '%fiftyville%'
)
ORDER BY hour ASC LIMIT 1;

-- checking if some person from those leaving the bakery are scheduled for this flight
SELECT * FROM passengers
WHERE flight_id = (
    SELECT id FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
(
    SELECT id FROM airports
    WHERE full_name LIKE '%fiftyville%'
)
ORDER BY hour ASC LIMIT 1
) AND passport_number IN (
    SELECT passport_number FROM people
    WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
)
);

-- crossing the flights info, phone info and atm info
SELECT * FROM people
WHERE passport_number IN
(
    SELECT passport_number FROM passengers
WHERE flight_id = (
    SELECT id FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
(
    SELECT id FROM airports
    WHERE full_name LIKE '%fiftyville%'
)
ORDER BY hour ASC LIMIT 1
) AND passport_number IN (
    SELECT passport_number FROM people
    WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
)
)
) AND
phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE caller IN (
    SELECT phone_number FROM people
    WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
)
) AND month = 7 AND day = 28
) AND id = (
 SELECT person_id FROM bank_accounts
    WHERE account_number IN (
    SELECT account_number FROM atm_transactions
    WHERE day = 28 AND month = 7 AND atm_location LIKE '%Leggett Street%' AND transaction_type = 'withdraw'
)
);

-- still 4 suspects. Checking the ATM as per interview
SELECT person_id FROM bank_accounts
WHERE account_number IN (
    SELECT account_number FROM atm_transactions
    WHERE day = 28 AND month = 7 AND atm_location LIKE '%Leggett Street%' AND transaction_type = 'withdraw'
);

-- crossing all the info in this query, the thief is bruce
SELECT * FROM people
WHERE passport_number IN
(
    SELECT passport_number FROM passengers
WHERE flight_id = (
    SELECT id FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
(
    SELECT id FROM airports
    WHERE full_name LIKE '%fiftyville%'
)
ORDER BY hour ASC LIMIT 1
) AND passport_number IN (
    SELECT passport_number FROM people
    WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
)
)
) AND
phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE caller IN (
    SELECT phone_number FROM people
    WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
)
) AND month = 7 AND day = 28
) AND id = (
 SELECT person_id FROM bank_accounts
    WHERE account_number IN (
    SELECT account_number FROM atm_transactions
    WHERE day = 28 AND month = 7 AND atm_location LIKE '%Leggett Street%' AND transaction_type = 'withdraw'
)
);

-- using the phone info, it's possible to find who helped him
SELECT * FROM people
WHERE phone_number =
(
SELECT receiver FROM phone_calls
WHERE caller = (

SELECT phone_number FROM people
WHERE passport_number IN
(
    SELECT passport_number FROM passengers
WHERE flight_id = (
    SELECT id FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
(
    SELECT id FROM airports
    WHERE full_name LIKE '%fiftyville%'
)
ORDER BY hour ASC LIMIT 1
) AND passport_number IN (
    SELECT passport_number FROM people
    WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
)
)
) AND
phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE caller IN (
    SELECT phone_number FROM people
    WHERE license_plate IN (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28 AND month = 7 AND hour = 10 AND minute > 15 AND activity = 'exit'
)
) AND month = 7 AND day = 28
) AND id = (
 SELECT person_id FROM bank_accounts
    WHERE account_number IN (
    SELECT account_number FROM atm_transactions
    WHERE day = 28 AND month = 7 AND atm_location LIKE '%Leggett Street%' AND transaction_type = 'withdraw'
)
)

)
AND month = 7 AND day = 28 AND duration < 60

);

-- finally to find the city Bruce escaped to
SELECT city FROM airports
WHERE id = (

    SELECT destination_airport_id FROM flights WHERE id = (
        SELECT id FROM flights WHERE day = 29 AND month = 7 AND origin_airport_id =
(
    SELECT id FROM airports
    WHERE full_name LIKE '%fiftyville%'
)
ORDER BY hour ASC LIMIT 1
    )
);