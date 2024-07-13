//go:build (linux && cgo) || (darwin && cgo)
// +build linux,cgo darwin,cgo

package main

// #cgo CFLAGS: -I../include -I/usr/local/include/mimalloc-2.1
// #cgo LDFLAGS: -L.. -L/usr/local/lib -ljson_flatten -lmimalloc
// #include "../include/json_flatten.h"
// #include <mimalloc.h>
import "C"
import (
	"reflect"
	"unsafe"
)

func GoString2CCharStar(str string) *C.char {
	return C.CString(str)
}

func CCharStar2GoString(s *C.char) string {
	return C.GoString(s)
}

func CCharStarN2GoString(s *C.char, n C.int) string {
	return C.GoStringN(s, n)
}

func GoBytes2CArray(bs []byte) unsafe.Pointer {
	return C.CBytes(bs)
}

func CArray2GoBytes(arr unsafe.Pointer, n C.int) []byte {
	return C.GoBytes(arr, n)
}

func main() {
	go_json_str := "{\"login\":\"Juminiy\",\"id\":58465840,\"node_id\":\"MDQ6VXNlcjU4NDY1ODQw\",\"avatar_url\":\"https://avatars.githubusercontent.com/u/58465840?v=4\",\"gravatar_id\":\"\",\"url\":\"https://api.github.com/users/Juminiy\",\"html_url\":\"https://github.com/Juminiy\",\"followers_url\":\"https://api.github.com/users/Juminiy/followers\",\"following_url\":\"https://api.github.com/users/Juminiy/following{/other_user}\",\"gists_url\":\"https://api.github.com/users/Juminiy/gists{/gist_id}\",\"starred_url\":\"https://api.github.com/users/Juminiy/starred{/owner}{/repo}\",\"subscriptions_url\":\"https://api.github.com/users/Juminiy/subscriptions\",\"organizations_url\":\"https://api.github.com/users/Juminiy/orgs\",\"repos_url\":\"https://api.github.com/users/Juminiy/repos\",\"events_url\":\"https://api.github.com/users/Juminiy/events{/privacy}\",\"received_events_url\":\"https://api.github.com/users/Juminiy/received_events\",\"type\":\"User\",\"site_admin\":false,\"name\":\"Chisato\",\"company\":null,\"blog\":\"https://2084team.com\",\"location\":\"Beijing\",\"email\":null,\"hireable\":null,\"bio\":\"wheeling making, lifelong learner\",\"twitter_username\":null,\"public_repos\":33,\"public_gists\":1,\"followers\":23,\"following\":76,\"created_at\":\"2019-12-03T09:32:07Z\",\"updated_at\":\"2024-07-12T00:27:58Z\"}"
	go_json_str_stable_ptr := (*reflect.StringHeader)(unsafe.Pointer(&go_json_str))

	// *C.char
	c_res := C.json_flatten_of((*C.char)(unsafe.Pointer(go_json_str_stable_ptr.Data)))
	defer C.free(unsafe.Pointer(c_res))
	println(CCharStar2GoString(c_res))
}
