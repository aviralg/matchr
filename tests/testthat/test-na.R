
test_that("NA values match correspondingly typed NA patterns", {

    na_list <- list(NA_character_, NA_integer_, NA_real_, NA)

    constructs <- expression(
        general = matchr::match(VALUE,
                                PATTERN ~ TRUE,
                                . ~ FALSE),
        general = matchr::match(VALUE,
                                vector(PATTERN) ~ TRUE,
                                . ~ FALSE),
        general = matchr::match(VALUE,
                                vec(PATTERN) ~ TRUE,
                                . ~ FALSE),
        character = matchr::match(VALUE,
                                  character(PATTERN) ~ TRUE,
                                  . ~ FALSE),
        character = matchr::match(VALUE,
                                  chr(PATTERN) ~ TRUE,
                                  . ~ FALSE),
        integer = matchr::match(VALUE,
                                integer(PATTERN) ~ TRUE,
                                . ~ FALSE),
        integer = matchr::match(VALUE,
                                int(PATTERN) ~ TRUE,
                                . ~ FALSE),
        double = matchr::match(VALUE,
                               double(PATTERN) ~ TRUE,
                               . ~ FALSE),
        double = matchr::match(VALUE,
                               dbl(PATTERN) ~ TRUE,
                               . ~ FALSE),
        double = matchr::match(VALUE,
                               real(PATTERN) ~ TRUE,
                               . ~ FALSE),
        logical = matchr::match(VALUE,
                                logical(PATTERN) ~ TRUE,
                                . ~ FALSE),
        logical = matchr::match(VALUE,
                                lgl(PATTERN) ~ TRUE,
                                . ~ FALSE)
    )

    for(VALUE in na_list)  {

        for(PATTERN in na_list) {

            for(index in 1:length(constructs)) {

                CONSTRUCT <- constructs[[index]]
                type <- names(constructs)[index]

                construct <- eval(substitute(substitute(CONSTRUCT)))

                result <- eval(construct)

                if((typeof(VALUE) == typeof(PATTERN)) && (type == "general" || type == typeof(VALUE))) {
                    expect_true(result)
                }
                else {
                    expect_false(result)
                }
            }

        }

    }
})
