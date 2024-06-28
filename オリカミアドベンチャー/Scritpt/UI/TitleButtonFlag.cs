using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using static Unity.IO.LowLevel.Unsafe.AsyncReadManagerMetrics;

public class TitleButtonFlag : MonoBehaviour
{
    public GameObject image;
    public Image m_DrawTex;

    public bool DontDestroyEnabled = true;

    float time = 0;

    private void Start()
    {
        DontDestroyOnLoad(gameObject);
    }

    private void Update()
    {
        if (DontDestroyEnabled) return;

        time += Time.deltaTime;

        Time.timeScale = 1.0f;

        if (time > 2.0f) SceneManager.MoveGameObjectToScene(gameObject,SceneManager.GetActiveScene());
    }

    public void Botton()
    {
        StartCoroutine("Capture");
    }

    public void PlayButton()
    {
        StartCoroutine("PlayCapture");
    }

    IEnumerator Capture()
    {
        //ReadPicxelsがこの後でないと使えないので必ず書く
        yield return new WaitForEndOfFrame();

        //スクリーンの大きさのSpriteを作る
        var texture = new Texture2D(Screen.width, Screen.height);

        //スクリーンを取得する
        texture.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0);
        //適応する
        texture.Apply();

        //取得した画像をSpriteに入るように変換する
        byte[] pngdata = texture.EncodeToPNG();
        texture.LoadImage(pngdata);

        //先ほど作ったSpriteに画像をいれる
        Sprite sprite = Sprite.Create(texture, new Rect(0, 0, texture.width, texture.height), new Vector2(0.5f, 0.5f));
        Debug.Log("c");

        //Spriteを使用するオブジェクトに指定する
        //     今回はUIのImage
        m_DrawTex.GetComponent<Image>().sprite = sprite;

        // サイズ変更
        m_DrawTex.GetComponent<RectTransform>().sizeDelta = new Vector2(texture.width, texture.height);

        //imageをアクティブにする
        image.SetActive(true);

        nextScene();
    }

    IEnumerator PlayCapture()
    {
        //ReadPicxelsがこの後でないと使えないので必ず書く
        yield return new WaitForEndOfFrame();

        //スクリーンの大きさのSpriteを作る
        var texture = new Texture2D(Screen.width, Screen.height);

        //スクリーンを取得する
        texture.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0);
        //適応する
        texture.Apply();

        //取得した画像をSpriteに入るように変換する
        byte[] pngdata = texture.EncodeToPNG();
        texture.LoadImage(pngdata);

        //先ほど作ったSpriteに画像をいれる
        Sprite sprite = Sprite.Create(texture, new Rect(0, 0, texture.width, texture.height), new Vector2(0.5f, 0.5f));
        Debug.Log("c");

        //Spriteを使用するオブジェクトに指定する
        //     今回はUIのImage
        m_DrawTex.GetComponent<Image>().sprite = sprite;

        // サイズ変更
        m_DrawTex.GetComponent<RectTransform>().sizeDelta = new Vector2(texture.width, texture.height);

        //imageをアクティブにする
        image.SetActive(true);
    }

    public void nextScene()
    {
        //destroyできるようにする
        DontDestroyEnabled = false;
    }

    public void StageSelectScene()
    {
        Time.timeScale = 1.0f;

        SceneManager.LoadScene("StageSelect");
    }

    //終了ボタン
    public void ExitButton()
    {
        Application.Quit();
    }
}
