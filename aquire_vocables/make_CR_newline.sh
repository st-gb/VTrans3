
echo using file $1

#http://linux.dsplabs.com.au/rmnl-remove-new-line-characters-tr-awk-perl-sed-c-cpp-bash-python-xargs-ghc-ghci-haskell-sam-ssam-p65/
tr -d '\r' < $1
