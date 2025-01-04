package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"net/http"
	"net/url"
	"time"
	"unicode/utf8"

	"github.com/gin-gonic/gin"
)

type Location struct {
	Name      string `json:"name"`
	ID        string `json:"id"`
	Lat       string `json:"lat"`
	Lon       string `json:"lon"`
	Adm2      string `json:"adm2"`
	Adm1      string `json:"adm1"`
	Country   string `json:"country"`
	TZ        string `json:"tz"`
	UtcOffset string `json:"utcOffset"`
	IsDst     string `json:"isDst"`
	Type      string `json:"type"`
	Rank      string `json:"rank"`
	FxLink    string `json:"fxLink"`
}

type location_list struct {
	Code     string     `json:"code"`
	Location []Location `json:"location"`
	Refer    struct {
		Sources []string `json:"sources"`
		License []string `json:"license"`
	} `json:"refer"`
}

type Daily struct {
	FxDate         string `json:"fxDate"`
	Sunrise        string `json:"sunrise"`
	Sunset         string `json:"sunset"`
	Moonrise       string `json:"moonrise"`
	Moonset        string `json:"moonset"`
	MoonPhase      string `json:"moonPhase"`
	MoonPhaseIcon  string `json:"moonPhaseIcon"`
	TempMax        string `json:"tempMax"`
	TempMin        string `json:"tempMin"`
	IconDay        string `json:"iconDay"`
	TextDay        string `json:"textDay"`
	IconNight      string `json:"iconNight"`
	TextNight      string `json:"textNight"`
	Wind360Day     string `json:"wind360Day"`
	WindDirDay     string `json:"windDirDay"`
	WindScaleDay   string `json:"windScaleDay"`
	WindSpeedDay   string `json:"windSpeedDay"`
	Wind360Night   string `json:"wind360Night"`
	WindDirNight   string `json:"windDirNight"`
	WindScaleNight string `json:"windScaleNight"`
	WindSpeedNight string `json:"windSpeedNight"`
	Humidity       string `json:"humidity"`
	Precip         string `json:"precip"`
	Pressure       string `json:"pressure"`
	Vis            string `json:"vis"`
	Cloud          string `json:"cloud"`
	UvIndex        string `json:"uvIndex"`
}

type weather_list struct {
	Code       string  `json:"code"`
	UpdateTime string  `json:"updateTime"`
	FxLink     string  `json:"fxLink"`
	Daily      []Daily `json:"daily"`
	Refer      struct {
		Sources []string `json:"sources"`
		License []string `json:"license"`
	} `json:"refer"`
}

func main() {
	KEY := "YOUR KEY"
	route := gin.Default()

	route.GET("/weather", func(ctx *gin.Context) {
		location := ctx.Query("location")
		// fmt.Println(location)

		resp, err := http.Get(fmt.Sprintf("https://geoapi.qweather.com/v2/city/lookup?key=%s&location=%s", KEY. url.QueryEscape(location)))
		if err != nil {
			fmt.Println(err)
			return
		}

		body, err := ioutil.ReadAll(resp.Body)
		// fmt.Println(string(body))
		// fmt.Println(resp.StatusCode)
		if resp.StatusCode == http.StatusNotFound {
			ctx.String(404, "NOT FOUND")
		} else if resp.StatusCode != http.StatusOK {
			ctx.String(500, string(body))
		}

		var res location_list

		// 解析 JSON 字符串
		err = json.Unmarshal(body, &res)
		if err != nil {
			ctx.String(500, err.Error())
		}

		if len(res.Location) == 0 {
			ctx.String(404, "NOT FOUND")
		}

		firstLocationID := res.Location[0].ID
		// ctx.String(200, firstLocationID)
		resp.Body.Close()

		// ctx.String(200, string(body))
		resp, err = http.Get(fmt.Sprintf("https://devapi.qweather.com/v7/weather/7d?key=%s&location=%s", KEY, firstLocationID))
		if err != nil {
			fmt.Println(err)
			return
		}

		body, err = ioutil.ReadAll(resp.Body)
		// fmt.Println(string(body))
		// fmt.Println(resp.StatusCode)
		if resp.StatusCode == http.StatusNotFound {
			ctx.String(404, "NOT FOUND")
		} else if resp.StatusCode != http.StatusOK {
			ctx.String(500, string(body))
		}

		// print(string(body))

		// 定义存储 JSON 解析结果的结构体
		var weather weather_list

		// 解析 JSON 字符串
		err = json.Unmarshal(body, &weather)
		if err != nil {
			log.Fatal(err)
		}

		var Result = ""
		for _, day := range weather.Daily {
			dayTextLen := utf8.RuneCountInString(day.TextDay)

			// 格式化输出
			Result += fmt.Sprintf("%-10s  %-*s %3s°C %3s°C\n", day.FxDate, 10-(dayTextLen), day.TextDay, day.TempMin, day.TempMax)
		}
		fmt.Println(Result)
		ctx.String(200, Result)
		defer resp.Body.Close()
	})

	route.Run(":12358") // 监听并在 0.0.0.0:12358 上启动服务
}
